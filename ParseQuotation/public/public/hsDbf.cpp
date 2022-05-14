#include "hsDbf.h"
#include "ifutils.h"

ThsDBF::ThsDBF()
{
  sFileName = NULL;
  fh = INVALID_HANDLE_VALUE;
  memset(&dbfHeader, 0, sizeof(dbfHeader));
  sBuffer = NULL;
  iFieldCount = 0;
  iRecordCount = 0;
  iBufferOffSet = 0;
  iBufferInterOffSet = 0;
  iBufferSize = 1;
  fieldList=NULL;
  bOpened = false;
  iValidBufferRec = 0;
  FiTestFlag = 0;
  Deleted = false;
  ResetError();
}

ThsDBF::~ThsDBF()
{
  Close();
}

bool ThsDBF::Open(const char* AFileName, unsigned short AMode)
{
  for (int i=0; i < OPEN_RETRY_TIMES; i++) 
	{
    if (OpenInternal(AFileName, AMode)) 
		{
      return true;
    } 
		else 
		{
      Sleep(RETRY_DELAY);
    }
  }
  return false;
}

bool ThsDBF::OpenInternal(const char* AFileName, unsigned short AMode)
{
  unsigned long shareMode=0, accessMode=0;
  unsigned long iLen;

  try 
	{
    this->Close();
    bReadOnly = false;
    if (fmRead & AMode) 
		{
      accessMode = accessMode|GENERIC_READ;
      bReadOnly = true;
    }
    if (fmWrite & AMode) 
		{
      accessMode = accessMode|GENERIC_WRITE;
      bReadOnly = false;
    }
    if (fmShare & AMode)
		{
			shareMode = FILE_SHARE_WRITE|FILE_SHARE_READ;
		}
    
		this->fh = CreateFile(AFileName, accessMode, shareMode,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  
		if (fh == INVALID_HANDLE_VALUE) 
		{
      DbfError(ERR_OPENFILE, "打开文件失败，请检查文件是否存在或被其他用户锁住!");
      return false;
    }

    if (! ReadFile(this->fh, (char *)&dbfHeader, 32, &iLen, NULL))
		{
      DbfError(ERR_READHEADER, "读DBF文件头信息失败，请检查文件是否被其他用户锁住");
      CloseHandle(this->fh);
      return false;
    }

    if (dbfHeader.iHeadSize % 32 != 1) 
		{
      DbfError(ERR_BADDBF, "err1 DBF文件格式不正确，请检查文件是否为Foxpro2.5格式的DBF");
      CloseHandle(this->fh);
      return false;
    }

    if ((iFieldCount = dbfHeader.iHeadSize / 32 - 1) < 1)  
		{
      DbfError(ERR_BADDBF, "err2 DBF文件格式不正确，请检查文件是否为Foxpro2.5格式的DBF");
      CloseHandle(this->fh);
      return false;
    }

    fieldList = new ThsFieldInfo[iFieldCount];
    if (fieldList == NULL) 
		{
      DbfError(ERR_NOMEMORY, "分配内存失败, 请检查内存使用情况");
      CloseHandle(this->fh);
      return false;
    }
    memset(fieldList, 0, sizeof(ThsFieldInfo) * iFieldCount);

    if ((! ReadFile(this->fh, (char *)fieldList, iFieldCount * sizeof(ThsFieldInfo), &iLen, NULL)) ||
        (iLen != iFieldCount * sizeof(ThsFieldInfo)) ) 
		{
      DbfError(ERR_READFIELD, "读取文件失败, 请检查文件是否被其他文件锁住");
      CloseHandle(this->fh);
      return false;
    }

    // adjust dbf field offset
    int iOffset=1;
    ThsFieldInfo *tmpField=this->fieldList;
    for (int i=0; i< iFieldCount; i++) 
		{
      tmpField->iFieldOffset = iOffset;
      iOffset = tmpField->iFieldOffset + tmpField->iLength;
      tmpField++;
    }

    if (! SetBufferSize(this->iBufferSize) ) 
		{
      CloseHandle(this->fh);
      return false;
    }
    bOpened = true;
    FState = tsBrowse;
    ReadBuffer(0);
    First();
/*
    if ((fmWrite|mode) && (! AdjustRecordNum())) {
      CloseHandle(this->fh);
      return false;
    }
*/
  } catch(...) 
	{
    return false;
  }
	
	//sFileName = _strdup(AFileName);
  sFileName = _strdup(AFileName);

  return true;
}

void ThsDBF::Close(void)
{
  if (bOpened) {
    CloseHandle(fh);
    bOpened = false;
  }

  if (sBuffer != NULL) {
    delete[] sBuffer;
    sBuffer = NULL;
  }

  if (fieldList) {
    delete[] fieldList;
    fieldList = NULL;
  }

  if (sFileName) {
    delete[] sFileName;
    sFileName = NULL;
  }

  fh = INVALID_HANDLE_VALUE;
  memset(&dbfHeader, 0, sizeof(dbfHeader));
  iFieldCount = 0;
  iRecordCount = 0;
  iBufferOffSet = 0;
  iBufferInterOffSet = 0;
  bOpened = false;
  iValidBufferRec = 0;
}

bool ThsDBF::GetField(char *AFieldName, char *AValue)
{
  if (!CheckOpen("COP001."))
	{
		return false;
	}
  if ((FState == tsBrowse) && (iValidBufferRec <= iBufferInterOffSet))
	{
		return false;
	}

  ThsFieldInfo *tmpField= GetFieldInfo(AFieldName);
  if (tmpField == NULL)
	{
		return false;
	}

  strncpy(AValue, sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, tmpField->iLength);
	//strcpy(AValue, sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset);
  AValue[tmpField->iLength] = 0;

  return true;
}

bool ThsDBF::GetField(char *AFieldName, int &AValue)
{
  char buff[100];
	//string sValue;
  memset(buff, 0, 100);
  if (GetField(AFieldName, buff))
	{
		AValue = atoi(buff);
    return true;
  } else
	{
    return false;
  }
}

bool ThsDBF::GetField(char *AFieldName, __int64 &AValue)
{
  char buff[100];
  memset(buff, 0, 100);
  if (GetField(AFieldName, buff))
	{
    AValue = _atoi64(buff);
    return true;
  } else 
	{
    return false;
  }
}

bool ThsDBF::GetField(char *AFieldName, double &AValue)
{
  char buff[100];
  memset(buff, 0, 100);
  if (GetField(AFieldName, buff))
	{
    AValue = atof(buff);
    return true;
  } else
	{
    return false;
  }
}

bool ThsDBF::GetField(char *AFieldName, bool &AValue)
{
  char buff[100];
  memset(buff, 0, 100);
  if (GetField(AFieldName, buff))
	{
    if (strlen(buff) == 1)
		{
      AValue = (buff[0] == 'T');
      return true;
    }
		else
		{
			return false;
		}
  }
	else 
	{
    return false;
  }
}

bool ThsDBF::SetField(char *AFieldName,const char *AValue)
{
  //if (iValidBufferRec <= iBufferInterOffSet) return false;
  if (!CheckOpen("COP002."))
	{
		return false;
	}
  if ((FState != tsEdit) && (FState != tsAppend))
	{
		return false;
	}

  ThsFieldInfo *tmpField= GetFieldInfo(AFieldName);
  if (tmpField == NULL)
	{
		return false;
	}
  memset(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, 0x20, tmpField->iLength);
  if (strlen(AValue) > tmpField->iLength)
	{
		memcpy(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, AValue, tmpField->iLength);
	}    
  else
	{
		memcpy(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, AValue, strlen(AValue));
	}
    
  return true;
}

bool ThsDBF::SetField(char *AFieldName, int AValue)
{
  if (!CheckOpen("COP003."))
	{
		return false;
	}
  if ((FState != tsEdit) && (FState != tsAppend))
	{
		return false;
	}

  char buff[100], fmt[100];
  ThsFieldInfo *tmpField= GetFieldInfo(AFieldName);
  if (tmpField == NULL)
	{
		return false;
	}
  memset(buff, 0, 100);
  memset(fmt, 0, 100);
  //sprintf(fmt, "%%%dd", tmpField->iLength);
	sprintf_s(fmt, "%%%dd", tmpField->iLength);
  //sprintf(buff, fmt, AValue);
	sprintf_s(buff, fmt, AValue);
  memcpy(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, buff, tmpField->iLength);

  return true;
}

bool ThsDBF::SetField(char *AFieldName, double AValue)
{
  if (!CheckOpen("COP004."))
	{
		return false;
	}
  if ((FState != tsEdit) && (FState != tsAppend))
	{
		return false;
	}

  char buff[100], fmt[100];
  ThsFieldInfo *tmpField= GetFieldInfo(AFieldName);
  if (tmpField == NULL)
	{
		return false;
	}
  memset(buff, 0, 100);
  memset(fmt, 0, 100);
  //sprintf(fmt, "%%%d.%df", tmpField->iLength, tmpField->iDec);
	sprintf_s(fmt, "%%%d.%df", tmpField->iLength, tmpField->iDec);
  //sprintf(buff, fmt, AValue);
	sprintf_s(buff, fmt, AValue);
  memcpy(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, buff, tmpField->iLength);

  return true;
}

bool ThsDBF::SetField(char *AFieldName, bool AValue)
{
  if (!CheckOpen("COP005."))
	{
		return false;
	}
  if ((FState != tsEdit) && (FState != tsAppend))
	{
		return false;
	}

  ThsFieldInfo *tmpField= GetFieldInfo(AFieldName);
  if (tmpField == NULL)
	{
		return false;
	}
  memset(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset, 0x20, tmpField->iLength);
  *(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength + tmpField->iFieldOffset) = (AValue ? 'T' : 'F');

  return true;
}

bool ThsDBF::Post(void)
{
  bool bret=false;
  char sDbfEof[2] = "\x1A";
  unsigned long iLen,iErr;

  ResetError();
  if (!CheckOpen("COP006."))
	{
		return false;
	}

  if (FState == tsEdit)
	{
    bret = LockRec(iBufferOffSet+iBufferInterOffSet+1);
    if (bret)
    {
      bret = (SetFilePointer(this->fh, dbfHeader.iHeadSize + dbfHeader.iRecordLength * (iBufferOffSet+iBufferInterOffSet), NULL, FILE_BEGIN) != 0xFFFFFFFF);
      if (bret)
      {
        bret = (WriteFile(this->fh, sBuffer + dbfHeader.iRecordLength * iBufferInterOffSet, dbfHeader.iRecordLength, &iLen, NULL) != 0);
        if (!bret)
				{
					iErr = GetLastError();
					//CHSLog::GetHSLog()->hsWriteError("LXT错误代码 %ld",iErr);
					DbfError(ERR_WRITEFILE, "9000.");
				}
      }
      else
			{
				DbfError(ERR_SETFILEPOINTER,"8001.");
			}
        
      FlushFileBuffers(this->fh);
      UnlockRec(iBufferOffSet+iBufferInterOffSet+1);
    }
  }
  else if (FState == tsAppend)
	{
    bret = LockAppend();
    int iRecCount = RecordCount();
    if (bret)
    {
      bret = (SetFilePointer(this->fh, dbfHeader.iHeadSize + dbfHeader.iRecordLength * iRecCount, NULL, FILE_BEGIN) != 0xFFFFFFFF);

      if (bret)
      {
        bret = (WriteFile(this->fh, sBuffer + dbfHeader.iRecordLength * iBufferInterOffSet, dbfHeader.iRecordLength, &iLen, NULL) != 0);
        if (bret)
				{
          bret = (WriteFile(this->fh, sDbfEof, 1, &iLen, NULL) != 0);

          if (bret)
					{
            if (SetFilePointer(this->fh, 4, NULL, FILE_BEGIN) != 0xFFFFFFFF)
						{
              iRecCount++;
              bret = (WriteFile(this->fh, (char *)&iRecCount, 4, &iLen, NULL) != 0);
              if (!bret)
                DbfError(ERR_WRITEFILE,"9001.");
            }
            else
						{
							DbfError(ERR_SETFILEPOINTER,"8001.");
						}            
          }
          else
					{
						DbfError(ERR_WRITEFILE,"9002.");
					}            
        }
        else
				{
					DbfError(ERR_WRITEFILE, "9003.");
				}          
      }
      else
			{
				DbfError(ERR_SETFILEPOINTER,"8002.");
			}
        

      bret = (FlushFileBuffers(this->fh) != 0);
      if (!bret) DbfError(ERR_FLUSHFILEBUFFERS, "11001.");
      UnlockAppend();
    }
  }

  if (bret) 
	{
    if (FState == tsAppend) 
		{
      FState = tsBrowse;
      Last();
    }
		else
		{
			FState = tsBrowse;
		}      
  }

  return bret;
}

bool ThsDBF::SetBufferSize(int ARecCount)
{
  if (ARecCount <= 0) 
	{
		ARecCount = 1;
	}

  if (bOpened)
	{
		return false;
	}
		
  this->iBufferSize = ARecCount;

  if (dbfHeader.iRecordLength <= 0)
	{
		return false;
	}
		
  if (sBuffer)
	{
    delete[] sBuffer;
    sBuffer = NULL;
  }

  sBuffer = new char[dbfHeader.iRecordLength*ARecCount + 1];
  memset(sBuffer, 0, dbfHeader.iRecordLength*ARecCount + 1);
  return (sBuffer != NULL);
}

void ThsDBF::First(void)
{
  if (!CheckOpen("COP007.")) 
	{
		return;
	}

  if ((FState == tsEdit) || (FState == tsAppend))
	{
		Cancel();
	}

  if (iBufferOffSet == 0)
	{
		iBufferInterOffSet = 0;
	}    
  else
	{
    //移动记录前，先清空缓存。
    memset(sBuffer, 0, dbfHeader.iRecordLength * iBufferSize + 1);
    iBufferOffSet = 0;
    iBufferInterOffSet = 0;
    ReadBuffer(iBufferInterOffSet);
  }

  if ((!Deleted) && IsDeletedRec()) Next();
}

void ThsDBF::Prior(void)
{
  if (!CheckOpen("COP008."))
	{
		return;
	}
  if ((FState == tsEdit) || (FState == tsAppend))
	{
		Cancel();
	}
  if (Bof())
	{
		return;
	}

  if (iBufferInterOffSet > 0)
	{
		iBufferInterOffSet--;
	}    
  else
	{
    //移动记录前，先清空缓存。
    memset(sBuffer, 0, dbfHeader.iRecordLength * iBufferSize + 1);
    iBufferOffSet -= iBufferSize;
    iBufferInterOffSet = iBufferSize-1;
    ReadBuffer(iBufferOffSet);
  }

  if ((! Deleted) && IsDeletedRec())
	{
		Prior();
	}
}

void ThsDBF::Next(void)
{
  if (!CheckOpen("COP009."))
	{
		return;
	}
  if ((FState == tsEdit) || (FState == tsAppend))
	{
		Cancel();
	}
  if (Eof())
	{
		return;
	}

  if (iBufferInterOffSet < iBufferSize - 1)
	{
		iBufferInterOffSet++;
	}    
  else 
	{
    //移动记录前，先清空缓存。
    memset(sBuffer, 0, dbfHeader.iRecordLength * iBufferSize + 1);
    iBufferOffSet += iBufferSize;
    iBufferInterOffSet = 0;
    ReadBuffer(iBufferOffSet);
  }

  if ((! Deleted) && IsDeletedRec())
	{
		Next();
	}
}


void ThsDBF::Last(void)
{
  if (!CheckOpen("COP010."))
	{
		return;
	}
  if ((FState == tsEdit) || (FState == tsAppend))
	{
		Cancel();
	}

  int cnt = RecordCount();
  if (iBufferOffSet == cnt/iBufferSize * iBufferSize)
	{
		iBufferInterOffSet = cnt - iBufferOffSet*iBufferSize;
	}    
  else
	{
    //移动记录前，先清空缓存。
    memset(sBuffer, 0, dbfHeader.iRecordLength * iBufferSize + 1);
    iBufferOffSet = (cnt-1)/iBufferSize * iBufferSize;
    iBufferInterOffSet = cnt - iBufferOffSet - 1;
    ReadBuffer(iBufferOffSet);
  }

  if ((! Deleted) && IsDeletedRec())
	{
		Prior();
	}
}

bool ThsDBF::Append(void)
{
  // 是否讲记录指针移到最后???
  if (FState == tsEdit)
	{
		return false;
	}

  FState = tsAppend;
  memset(sBuffer, 0, dbfHeader.iRecordLength*this->iBufferSize + 1);
  return true;
}

bool ThsDBF::Edit(void)
{
  if (FState == tsAppend)
	{
		return false;
	}
  FState = tsEdit;
  return true;
}

bool ThsDBF::Eof(void)
{
  if (!CheckOpen("COP011.")) 
	{
		return true;
	}
  if (iBufferInterOffSet < (this->iValidBufferRec-1))
	{
		return false;
	}
  
	return (iBufferOffSet + iBufferInterOffSet + 1 > RecordCount());
}

bool ThsDBF::Bof(void)
{
  if (!CheckOpen("COP012."))
	{
		return true;
	}
  return (iBufferOffSet + iBufferInterOffSet == 0);
}

bool ThsDBF::BufferIsOk(void)
{
  if (!CheckOpen("COP013."))
	{
		return false;
	}
  return (iValidBufferRec > iBufferInterOffSet);
}

int ThsDBF::RecordCount(void)
{
  int iRecCnt;
  unsigned long iLen;
  if (!CheckOpen("COP014."))
	{
		return 0;
	}

  if (bReadOnly) 
	{
    SetFilePointer(this->fh, 4, NULL, FILE_BEGIN);
    if (ReadFile(this->fh, &iRecCnt, 4, &iLen, NULL))
		{
			dbfHeader.iRecordNum = iRecCnt;
		}     
    else
		{
			return 0;
		}
  } 
	else
	{
    dbfHeader.iRecordNum = (long)((SetFilePointer(this->fh, 0, NULL, FILE_END) - dbfHeader.iHeadSize) / dbfHeader.iRecordLength);
  }

  return dbfHeader.iRecordNum;
}

bool ThsDBF::Opened(void)
{
  return bOpened;
}

bool ThsDBF::ReadBuffer(int ARecOffset)
{
  unsigned long iLen=0;
  bool bReadSuccess = false;
  char sMsg[1024];

  FState = tsBrowse;
  FiTestFlag = 100;

  if (!CheckOpen("COP015."))
  {
    FiTestFlag = -100;
    return false;
  }
  
  memset(sBuffer, 0, dbfHeader.iRecordLength * iBufferSize + 1);
  iValidBufferRec = 0;
  FiTestFlag = 200;

  //读取失败重试REC_LOCK_RETRY_TIMES次
  for(int i = 0; i < REC_LOCK_RETRY_TIMES; i++)
  {
    if ( SetFilePointer(this->fh, (long)dbfHeader.iHeadSize + (long)(dbfHeader.iRecordLength * ARecOffset),
                       NULL, FILE_BEGIN) == 0xFFFFFFFF)
    {
      DbfError(ERR_READFIELD, "移动位置失败");
      //标志读取记录失败
      FiTestFlag = -200;
      return false;
    }

    FiTestFlag = 200;
    //读取成功跳出循环
    iLen = 0;
    if (ReadFile(this->fh, sBuffer, dbfHeader.iRecordLength * iBufferSize, &iLen, NULL)) 
		{
      bReadSuccess = true;
      break;
    }
    //重试延时
    Sleep(REC_RETRY_DELAY);
  }

  FiTestFlag = 300;

  //读取失败，报读取记录超时的错误信息
  if (!bReadSuccess)
  {
    //sprintf(sMsg, "当前出错记录号[%d]，", ARecOffset);
		sprintf_s(sMsg, "当前出错记录号[%d]，", ARecOffset);
    DbfError(ERR_READREC_OUTTIME, sMsg);
    //标志读取记录失败
    FiTestFlag = -300; 
    return false;
  }

  iValidBufferRec = iLen / dbfHeader.iRecordLength;
  if  (iValidBufferRec <= 0)
  {
		//sprintf(sMsg, "DBF内容没读到,记录号[%d]，", ARecOffset);
		sprintf_s(sMsg, "DBF内容没读到,记录号[%d]，", ARecOffset);
    DbfError(ERR_READREC_OUTTIME, sMsg);
    return false;
  }

  //读取成功
  FiTestFlag = 1000;

  return true;
}

bool ThsDBF::IsDeletedRec(void)
{
  if (!CheckOpen("COP016."))
	{
		return false;
	}
  char cDeleteFlag = '\x2A';
  return (*(sBuffer + iBufferInterOffSet * dbfHeader.iRecordLength) == cDeleteFlag);
}

ThsFieldInfo * ThsDBF::GetFieldInfo(char *AFieldName)
{
  if (!CheckOpen("COP017."))
	{
		return NULL;
	}
  if (fieldList == NULL)
	{
		return NULL;
	}

  ThsFieldInfo *tmpField=this->fieldList;
  for (int i=0; i < iFieldCount; i++)
	{
//     if (stricmp(tmpField->sFieldName, AFieldName) == 0)
// 		{
// 			return tmpField;
// 		}
		if (_stricmp(tmpField->sFieldName, AFieldName) == 0)
		{
			return tmpField;
		}
    tmpField++;
  }

  return NULL;
}

void ThsDBF::Cancel(void)
{
  ReadBuffer(iBufferOffSet);
}

bool ThsDBF::LockDBF(void)
{
  if (!CheckOpen("COP018."))
	{
		return false;
	}
  UnlockDBF();
  for (int i = 0; i < LOCK_RETRY_TIMES; i++)
	{
    if (LockFile(this->fh, 0, 0, FILE_LOCK_LEN, 0))
		{
			return true;
		}
    else Sleep(RETRY_DELAY);
  }
  return false;
}

bool ThsDBF::UnlockDBF(void)
{
  if (!CheckOpen("COP019."))
	{
		return false;
	}
  return (UnlockFile(this->fh, 0, 0, FILE_LOCK_LEN, 0) != 0);
}

bool ThsDBF::LockRec(unsigned int ARecNo, bool ALockHeader)
{
  bool bret = false;
  if (ARecNo <= 0)
	{
		return false;
	}
  if (!CheckOpen("COP020."))
	{
		return false;
	}

  for (int i = 0; i < LOCK_RETRY_TIMES; i++)
	{
    if (LockFile(this->fh, dbfHeader.iHeadSize + (ARecNo - 1) * dbfHeader.iRecordLength, 0, dbfHeader.iRecordLength, 0))
		{
      bret = true;
      break;
    }
		else
		{
			Sleep(RETRY_DELAY);
		}      
  }

  if (ALockHeader && bret)
	{
    for (int i = 0; i < LOCK_RETRY_TIMES; i++)
		{
      if (LockFile(this->fh, 0, 0, 32, 0))
			{
				return true;
			}        
      else
			{
				Sleep(RETRY_DELAY);
			}        
    }
    return false;
  }
  if (!bret)
	{
		DbfError(ERR_LOCKFAILED_EDIT, "6000.");
	}
       
  return bret;
}

bool ThsDBF::UnlockRec(unsigned int ARecNo, bool AUnlockHeader)
{
  bool bret;
  if (!CheckOpen("COP021.")) return false;

  bret = (UnlockFile(this->fh, dbfHeader.iHeadSize + (ARecNo - 1) * dbfHeader.iRecordLength, 0, dbfHeader.iRecordLength, 0) != 0);
  if (AUnlockHeader && bret)
    return (UnlockFile(this->fh, 0, 0, 32, 0) != 0);

  return bret;
}

bool ThsDBF::LockAppend(void)
{
  bool bret = false;
  int irecCnt;

  if (!CheckOpen("COP022."))
	{
		return false;
	}
  
  iAppendLockStart = 0;
  for (int i = 0; i < LOCK_RETRY_TIMES; i++)
	{
    irecCnt = RecordCount();
    SetFilePointer(this->fh, 0, NULL, FILE_BEGIN);
    if (LockFile(this->fh, dbfHeader.iHeadSize +  irecCnt * dbfHeader.iRecordLength, 0, FILE_LOCK_LEN, 0))
		{
      bret = true;
      iAppendLockStart = dbfHeader.iHeadSize + irecCnt * dbfHeader.iRecordLength;
      break;
    }
		else
		{
			Sleep(RETRY_DELAY);
		}    
  }

  if (bret)
	{
    bool bOk = false;
    for (int i = 0; i < LOCK_RETRY_TIMES; i++)
		{
      SetFilePointer(this->fh, 0, NULL, FILE_BEGIN);
      if (LockFile(this->fh, 0, 0, 32, 0))
			{
        bOk = true;
        break;
      }
			else
			{
				Sleep(RETRY_DELAY);
			}       
    }

    if (! bOk)
		{
      UnlockFile(this->fh, iAppendLockStart, 0, FILE_LOCK_LEN, 0);
      bret = false;
    }
  }

  if (!bret)
	{
		DbfError(ERR_LOCKFAILED_APPEND, "7000.");
	}    
  return bret;
}

bool ThsDBF::UnlockAppend(void)
{
  bool bret;

  if (!CheckOpen("COP023."))
	{
		return false;
	}

  bret = (UnlockFile(this->fh, iAppendLockStart, 0, FILE_LOCK_LEN, 0) != 0);
  if (bret)
	{
		bret = (UnlockFile(this->fh, 0, 0, 32, 0) != 0);
	}

  return bret;
}


bool ThsDBF::GetFieldName(int AIndex, char *AName)
{
  if (!CheckOpen("COP024."))
	{
		return false;
	}
  if (fieldList == NULL)
	{
		return false;
	}
  if (AIndex >= iFieldCount)
	{
		return false;
	}

  ThsFieldInfo *tmpField=this->fieldList;
  tmpField += AIndex;
  strcpy(AName, tmpField->sFieldName);
//	strcpy_s(AName, tmpField->sFieldName);
  return true;
}

int ThsDBF::GetFieldCount(void)
{
  return iFieldCount;
}

void ThsDBF::Go(unsigned int RecNo)     // bug
{
  int itmp;
  FiTestFlag = 0;
  if (!CheckOpen("COP025."))
	{
		FiTestFlag = -10;
		return; 
	}
  if ((FState == tsEdit) || (FState == tsAppend))
	{
		Cancel();
	}

  itmp = (RecNo-1)/iBufferSize;
  if (iBufferOffSet == itmp*iBufferSize)
	{
		iBufferInterOffSet = RecNo - iBufferOffSet -1;
	}    
  else
	{
    //移动记录前，先清空缓存。
    memset(sBuffer, 0, dbfHeader.iRecordLength * iBufferSize + 1);

    FiTestFlag = 10;
    iBufferOffSet = itmp*iBufferSize;
    iBufferInterOffSet = RecNo - iBufferOffSet - 1;
    FiTestFlag = 20;

    ReadBuffer(iBufferOffSet);
  }
}

unsigned int ThsDBF::RecordNo(void)
{
  return iBufferOffSet + iBufferInterOffSet + 1;
}

bool ThsDBF::Zap(void)
{
  char tmppath[512], tmpname[1024];
  TDBFHeader header;
  HANDLE fnew;
  unsigned long iLen;

  memcpy(&header, &dbfHeader, sizeof(dbfHeader));
  header.iRecordNum = 0;

  if (GetTempPath(511, tmppath) == 0)
	{
    //strcpy(tmppath, ".\\");
		strcpy_s(tmppath, ".\\");
  }
  if (GetTempFileName(tmppath, "dbf", 0, tmpname) == 0)
	{
    return false;
  }

  fnew = CreateFile(tmpname, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                    NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (fnew == INVALID_HANDLE_VALUE) 
	{
    return false;
  } 
	else if (SetFilePointer(fnew, 0, NULL, FILE_END) != 0) 
	{
    CloseHandle(fnew);
    return false;
  }

  if ((! WriteFile(fnew, &header, sizeof(header), &iLen, NULL)) || (iLen != sizeof(header))) 
	{
    CloseHandle(fnew);
    DeleteFile(tmpname);
    return false;
  }
  if ((! WriteFile(fnew, fieldList, sizeof(ThsFieldInfo)*(header.iHeadSize / 32 - 1), &iLen, NULL)) ||
      (iLen != sizeof(ThsFieldInfo)*(header.iHeadSize / 32 - 1))) 
	{
    CloseHandle(fnew);
    DeleteFile(tmpname);
    return false;
  }
  if ((! WriteFile(fnew, "0x\0D", 1, &iLen, NULL) ) || (iLen != 1))
	{
    CloseHandle(fnew);
    DeleteFile(tmpname);
    return false;
  }

  char *sSrcFile;
  //sSrcFile = strdup(sFileName);
	sSrcFile = _strdup(sFileName);
  CloseHandle(fnew);
  this->Close();
  if (! (DeleteFile(sSrcFile) && MoveFile(tmpname, sSrcFile)))
	{
    DeleteFile(tmpname);
    delete[] sSrcFile;
    return false;
  }
  delete[] sSrcFile;

  return true;
}

bool ThsDBF::CheckOpen(char *sErrCode)
{
  if (!bOpened)
	{
		DbfError(ERR_NOT_OPEN, sErrCode);
	}    
  return bOpened;
}

void ThsDBF::DbfError(unsigned int iErrorCode, char *sErrMsgEx)
{
  iErrCode = iErrorCode;
  //sprintf(sErrMsg, "%s", sErrMsgEx);
	//strcpy(sErrMsg,sErrMsgEx);
	strcpy_s(sErrMsg,sErrMsgEx);
	//CHSLog::GetHSLog()->hsWriteError(sErrMsgEx);
}

void ThsDBF::ResetError()
{
  iErrCode = 0;
  memset(sErrMsg, 0, sizeof(sErrMsg));
}

char* ThsDBF::GetErrMsg()
{
  switch(iErrCode)
  {
//     case ERR_OPENFILE: strcat(sErrMsg, ERR_OPENFILE_MSG); break;
//     case ERR_READHEADER: strcat(sErrMsg, ERR_READHEADER_MSG); break;
//     case ERR_BADDBF: strcat(sErrMsg, ERR_BADDBF_MSG); break;
//     case ERR_READFIELD: strcat(sErrMsg, ERR_READFIELD_MSG); break;
//     case ERR_NOMEMORY: strcat(sErrMsg, ERR_NOMEMORY_MSG); break;
//     case ERR_LOCKFAILED_EDIT: strcat(sErrMsg, ERR_LOCKFAILED_EDIT_MSG); break;
//     case ERR_LOCKFAILED_APPEND: strcat(sErrMsg, ERR_LOCKFAILED_APPEND_MSG); break;
//     case ERR_SETFILEPOINTER: strcat(sErrMsg, ERR_SETFILEPOINTER_MSG); break;
//     case ERR_WRITEFILE: strcat(sErrMsg, ERR_WRITEFILE_MSG); break;
//     case ERR_NOT_OPEN: strcat(sErrMsg, ERR_NOT_OPEN_MSG); break;
//     case ERR_READREC_OUTTIME: strcat(sErrMsg, ERR_READREC_OUTTIME_MSG); break;
//     default: strcat(sErrMsg, ERR_DEFAULT_MSG);
	case ERR_OPENFILE: strcat_s(sErrMsg, ERR_OPENFILE_MSG); break;
	case ERR_READHEADER: strcat_s(sErrMsg, ERR_READHEADER_MSG); break;
	case ERR_BADDBF: strcat_s(sErrMsg, ERR_BADDBF_MSG); break;
	case ERR_READFIELD: strcat_s(sErrMsg, ERR_READFIELD_MSG); break;
	case ERR_NOMEMORY: strcat_s(sErrMsg, ERR_NOMEMORY_MSG); break;
	case ERR_LOCKFAILED_EDIT: strcat_s(sErrMsg, ERR_LOCKFAILED_EDIT_MSG); break;
	case ERR_LOCKFAILED_APPEND: strcat_s(sErrMsg, ERR_LOCKFAILED_APPEND_MSG); break;
	case ERR_SETFILEPOINTER: strcat_s(sErrMsg, ERR_SETFILEPOINTER_MSG); break;
	case ERR_WRITEFILE: strcat_s(sErrMsg, ERR_WRITEFILE_MSG); break;
	case ERR_NOT_OPEN: strcat_s(sErrMsg, ERR_NOT_OPEN_MSG); break;
	case ERR_READREC_OUTTIME: strcat_s(sErrMsg, ERR_READREC_OUTTIME_MSG); break;
	default: strcat_s(sErrMsg, ERR_DEFAULT_MSG);
  }
  
  return sErrMsg;
}

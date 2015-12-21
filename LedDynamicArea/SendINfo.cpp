#include "stdafx.h"
#include "SendInfo.h"

string GetNAckMessageInfo(DWORD nScreenOrd, Json::Value Screen_ja, string szSendMessage, DWORD nNAckOrd)
{
	string Result = "Error: ";
	char buffer[32];

	Result += _itoa(nNAckOrd, buffer, 10); // + '; ';
	switch (nNAckOrd)
	{
	case ERR_OUTOFGROUP:
		Result = Result; //+ _(' Command Group Error'); //Command Group Error
		break;
	case ERR_NOCMD:
		Result = Result; //+ _(' Command Not Found'); //Command Not Found
		break;
	case ERR_BUSY:
		Result = Result; // + _(' The Controller is busy now'); //The Controller is busy now
		break;
	case ERR_MEMORYVOLUME:
		Result = Result; //+ _(' Out of the Memory Volume'); // Out of the Memory Volume
		break;
	case ERR_CHECKSUM:
		Result = Result; //+ _(' CRC16 Checksum Error'); //CRC16 Checksum Error
		break;
	case ERR_FILENOTEXIST:
		Result = Result; //+ _(' File Not Exist'); // File Not Exist
		break;
	case ERR_FLASH:
		Result = Result; //+ _(' Flash Access Error'); //Flash Access Error
		break;
	case ERR_FILE_DOWNLOAD:
		Result = Result; //+ _(' File Download Error'); // File Download Error
		break;
	case ERR_FILE_NAME:
		Result = Result; //+ _(' File name Error'); //Filename Error
		break;
	case ERR_FILE_TYPE:
		Result = Result; //+ _(' File type Error'); // File type Error
		break;
	case ERR_FILE_CRC16:
		Result = Result; //+ _(' File CRC16 Error'); //File CRC16 Error
		break;
	case ERR_FONT_NOT_EXIST:
		Result = Result; //+ _(' Font Library Not Exist'); // Font Library Not Exist
		break;
	case ERR_FIRMWARE_TYPE:
		Result = Result; //+ _(' Firmware Type Error (Check the controller type)'); //Firmware Type Error (Check the controller type)
		break;
	case ERR_DATE_TIME_FORMAT:
		Result = Result; //+ _(' Date Time format error'); // Date Time format error
		break;
	case ERR_FILE_EXIST:
		Result = Result; //+ _(' File Exist for File overwrite'); //File Exist for File overwrite
		break;
	case ERR_FILE_BLOCK_NUM:
		Result = Result; // + _(' File block number error'); //File block number error
		break;
	case ERR_CONTROLLER_TYPE:
		Result = Result; //+ _(' Controller Type Error');
		break;
	case Err_SCREEN_PARA:
		Result = Result; //+ _(' Controller parameters or error bounds');
		break;
	case ERR_CONTROLLER_ID:
		Result = Result; //+ _(' Read controller ID error'); //读取控制器 ID错误
		break;
	case ERR_USER_SECRET:
		Result = Result; // + _(' The password software password is inconsistent with the controller'); //软件中设定的密码与控制器不一致
		break;
	case ERR_OLD_SECRET:
		Result = Result; //+ _(' Old password input error'); //旧密码输入错误
		break;
	case ERR_PHY1_NO_SECRET:
		Result = Result; // + _(' The controller does not set the password, use the normal-mode communication'); //控制器中没有设定密码请使用正常模式通讯
		break;
	case ERR_PHY1_USE_SECRET:
		Result = Result; //+ _(' Controller has set the password, please use the encryption mode over Communications'); //控制器已经设定过密码请使用加密模式通讯
		break;
	default: //ERRR_OTHER  Other Error;
		Result += " Other unknown error";
	}

	return Result;
}

DWORD GetAckOrNAck(tagstruct_PHY1Header srt_PHY1Header, DWORD nSendCmd, string szReadBuf,
	bool &bNeedReSend)
{
	DWORD nReadLength;
	DWORD CmdGroup, Cmd;
	DWORD Result = 0;

	bNeedReSend = true;
	nReadLength = szReadBuf.size();
	CmdGroup = (nSendCmd >> 8) & 0xFF;
	Cmd = nSendCmd & 0xFF;
	if (nSendCmd == SEND_ACK)
	{
		if (srt_PHY1Header.ProtocolVer == PROTOCOLVER_FIFTH_DYNAMIC)
		{
			if (nReadLength < 8)
			{
				bNeedReSend = true;
				return ERR_OTHER;
			}

			if (((BYTE)szReadBuf[2] == CmdGroup) && ((BYTE)szReadBuf[3] == Cmd))
			{
				Result = (BYTE)szReadBuf[6];
			}
			else if (((BYTE)szReadBuf[2] == 0xA0) && ((BYTE)szReadBuf[3] == 1))
			{
				Result = (BYTE)szReadBuf[6];
			}
		}
		else
			Result = ERR_OTHER;
	}

	switch (Result)
	{
	case ERR_FILE_DOWNLOAD:
	case Err_SCREEN_PARA:
	case ERR_FILE_CRC16:
	case ERR_FILENOTEXIST:
		bNeedReSend = false;
		break;
	default:
		bNeedReSend = true;
		break;
	}

	return Result;
}
#include "stdafx.h"
#include "Global_Function.h"
#include "Global_Data.h"

//procedure LanguageUse(AnObject: TComponent; lan_i: integer);
//begin
//TranslateComponent(AnObject);
//UseLanguage(LangCode[lan_i]);
////RetranslateComponent(AnObject);
//end;

void SaveBinFile(const char *pdata, UINT nDataLen, string szFileName)
{
	ofstream fp;

	/*if not DirectoryExists(ExtractFilePath(szFileName)) then
		CreateDir(ExtractFilePath(szFileName));*/
	fp.open(szFileName);
	fp.write(pdata, nDataLen);
	fp.close();
}

//procedure ClearTransitList(var lstTransit : TList);
//var
//ptmptagTransit : Ptag_Transit;
//begin
//while lstTransit.Count > 0 do
//begin
//ptmptagTransit : = lstTransit.Items[0];
//dispose(ptmptagTransit);
//lstTransit.Delete(0);
//end;
//end;

//function GetTransitScreenStatus(lstTransit: TList; ScreenOrd: Cardinal; Screen_Ary: TSuperArray) : Boolean;
//var
//tmpresult : Boolean;
//barcode, networkid: string;
//ptmptagtransit: Ptag_Transit;
//nsendmode, i: Cardinal;
//begin
//tmpresult : = False;
//nsendmode: = Screen_Ary[ScreenOrd].I[SO_Screen_SendMode];
//barcode: = Screen_Ary[ScreenOrd].S[SO_Transit_Barcode];
//networkid: = Screen_Ary[ScreenOrd].S[SO_Network_ID];
//if nsendmode = SEND_MODE_Server_2G then
//begin
//for i : = 1 to lstTransit.Count do
//begin
//ptmptagtransit : = lstTransit.Items[i - 1];
//if ptmptagtransit^.Transit_Type = Transit_Device_2G then
//begin
//if ptmptagtransit^.BarCode = barcode then
//begin
//if ptmptagtransit^.Status = Transit_Device_Online then
//tmpresult : = True;
//Break;
//end;
//end;
//end;
//end
//else if nsendmode = SEND_MODE_Server_3G then
//begin
//for i : = 1 to lstTransit.Count do
//begin
//ptmptagtransit : = lstTransit.Items[i - 1];
//if ptmptagtransit^.Transit_Type = Transit_Device_3G then
//begin
//if (ptmptagtransit^.BarCode = barcode)
//and (ptmptagtransit^.NetworkID = networkid) then
//begin
//if ptmptagtransit^.Status = Transit_Device_Online then
//tmpresult : = True;
//Break;
//end;
//end;
//end;
//end;
//Result: = tmpresult;
//end;

//function Judge_Scan_File_Detail_Exist(ScanObj: ISuperObject;
//szSmartScanPath: WideString; var szSameScanName : widestring) : Boolean;
//var
//AStrings : TTntStringList;
//nScan, I: Cardinal;
//tmpScanObj: ISuperObject;
//szFileName: WideString;
//szScan: wideString;
//function Judge_Con_Table_List_Same(ConTableList_Ary1, ConTableList_Ary2: TSuperArray) : Boolean; //True:一致；False：不一致。
//var
//I : Cardinal;
//begin
//Result : = True;
//if ConTableList_Ary1.Length <> ConTableList_Ary2.Length then
//Result : = False
//else //长度一致，再进行进一步的表值判断
//begin
//for I : = 1 to ConTableList_Ary1.Length do
//begin
//if ConTableList_Ary1[I - 1].I['Con_Table_Data'] <> ConTableList_Ary2[I - 1].I['Con_Table_Data'] then
//begin
//Result : = False;
//break;
//end;
//end;
//end;
//end;
//begin
//Result : = False;
//{szScan: = GetScanStyleValue(ScanObj.I['Scan']);
//AStrings: = TTntStringList.Create;
//AStrings.Clear;
//try
//GetFileList(AStrings, szSmartScanPath + szScan + '_' + '*";
//for I : = 1 to AStrings.Count do
//begin
//szFileName : = AStrings[I - 1];
//tmpScanObj: = TSuperObject.ParseFile(szFileName);
//if
//(ScanObj.I['Scan'] = tmpScanObj.I['Scan'])
//// and (ScanObj.I['EMIF_REG'] = tmpScanObj.I['EMIF_REG'])
//// and (ScanObj.I['Freq'] = tmpScanObj.I['Freq'])
//// and (ScanObj.I['Lat_sign_width'] = tmpScanObj.I['Lat_sign_width'])
//// and (ScanObj.I['OEFront'] = tmpScanObj.I['OEFront'])
//// and (ScanObj.I['disp_refresh'] = tmpScanObj.I['disp_refresh'])
//// and (ScanObj.I['ln_cont_mod'] = tmpScanObj.I['ln_cont_mod'])
//// and (ScanObj.I['GAM_R'] = tmpScanObj.I['GAM_R'])
//// and (ScanObj.I['GAM_G'] = tmpScanObj.I['GAM_G'])
//// and (ScanObj.I['GAM_B'] = tmpScanObj.I['GAM_B'])
//and (ScanObj.I['ChipID'] = tmpScanObj.I['ChipID'])
//and (ScanObj.I['RouteSeq'] = tmpScanObj.I['RouteSeq'])
//and (ScanObj.I['Decoder_unen'] = tmpScanObj.I['Decoder_unen'])
//and (ScanObj.I['DA'] = tmpScanObj.I['DA'])
//and (ScanObj.I['OE'] = tmpScanObj.I['OE'])
//and (ScanObj.I['Wu'] = tmpScanObj.I['Wu'])
//and (ScanObj.I['Hu'] = tmpScanObj.I['Hu'])
//and (ScanObj.I['ScreenColor'] = tmpScanObj.I['ScreenColor'])
//and (ScanObj.I['Chn'] = tmpScanObj.I['Chn'])
//and (ScanObj.I['RSN'] = tmpScanObj.I['RSN'])
//and (ScanObj.I['GSN'] = tmpScanObj.I['GSN'])
//and (ScanObj.I['BSN'] = tmpScanObj.I['BSN'])
//and (ScanObj.I['RowsPerChn'] = tmpScanObj.I['RowsPerChn'])
//and (ScanObj.I['FoldNum'] = tmpScanObj.I['FoldNum'])
//and (Judge_Con_Table_List_Same(ScanObj.A['Con_Table_List'], tmpScanObj.A['Con_Table_List']) = True)
//then
//begin
//Result : = true;
//szSameScanName: = tmpScanObj.S['ScanName'];
//break;
//end;
//end;
//finally
//FreeAndNil(AStrings);
//end; }
//end;

//function SaveScanData(SmartScanFile_root: ISuperObject; ScanObj: ISuperObject;
//szSmartScanPath, szcurScanName, szScanFile: Widestring) : Boolean;
//var
//nOrd : Cardinal;
//stream: TtntMemoryStream;
//szSmartScanFile: WideString;
//curSmartScan_root: ISuperObject;
//begin
//{ case ScanObj.I['Scan'] of
//SCAN_TYPE_SIXTEEN :
//begin
//SmartScanFile_root.A['SixTeenList'].Add(SO(ScanObj.AsJSon));
//nOrd: = SmartScanFile_root.A['SixTeenList'].Length - 1;
//SmartScanFile_root.A['SixTeenList'][nOrd].I['ConfNumber'] : = nOrd;
//SmartScanFile_root.A['SixTeenList'][nOrd].S['ScanName'] : = szcurScanName;
//szSmartScanFile: = '16_' + IntToStr(nOrd) + '.sc';
//curSmartScan_root: = SmartScanFile_root.A['SixTeenList'][nOrd];
//end;
//SCAN_TYPE_EIGHT:
//begin
//SmartScanFile_root.A['EightList'].Add(SO(ScanObj.AsJSon));
//nOrd: = SmartScanFile_root.A['EightList'].Length - 1;
//SmartScanFile_root.A['EightList'][nOrd].I['ConfNumber'] : = nOrd;
//SmartScanFile_root.A['EightList'][nOrd].S['ScanName'] : = szcurScanName;
//szSmartScanFile: = '8_' + IntToStr(nOrd) + '.sc';
//curSmartScan_root: = SmartScanFile_root.A['EightList'][nOrd];
//end;
//SCAN_TYPE_FOUR:
//begin
//SmartScanFile_root.A['FourList'].Add(SO(ScanObj.AsJSon));
//nOrd: = SmartScanFile_root.A['FourList'].Length - 1;
//SmartScanFile_root.A['FourList'][nOrd].I['ConfNumber'] : = nOrd;
//SmartScanFile_root.A['FourList'][nOrd].S['ScanName'] : = szcurScanName;
//szSmartScanFile: = '4_' + IntToStr(nOrd) + '.sc';
//curSmartScan_root: = SmartScanFile_root.A['FourList'][nOrd];
//end;
//SCAN_TYPE_TWO:
//begin
//SmartScanFile_root.A['TwoList'].Add(SO(ScanObj.AsJSon));
//nOrd: = SmartScanFile_root.A['TwoList'].Length - 1;
//SmartScanFile_root.A['TwoList'][nOrd].I['ConfNumber'] : = nOrd;
//SmartScanFile_root.A['TwoList'][nOrd].S['ScanName'] : = szcurScanName;
//szSmartScanFile: = '2_' + IntToStr(nOrd) + '.sc';
//curSmartScan_root: = SmartScanFile_root.A['TwoList'][nOrd];
//end;
//SCAN_TYPE_STATIC:
//begin
//SmartScanFile_root.A['StaticList'].Add(SO(ScanObj.AsJSon));
//nOrd: = SmartScanFile_root.A['StaticList'].Length - 1;
//SmartScanFile_root.A['StaticList'][nOrd].I['ConfNumber'] : = nOrd;
//SmartScanFile_root.A['StaticList'][nOrd].S['ScanName'] : = szcurScanName;
//szSmartScanFile: = '1_' + IntToStr(nOrd) + '.sc';
//curSmartScan_root: = SmartScanFile_root.A['StaticList'][nOrd];
//end;
//else
//begin
//SmartScanFile_root.A['OtherList'].Add(SO(ScanObj.AsJSon));
//nOrd: = SmartScanFile_root.A['OtherList'].Length - 1;
//SmartScanFile_root.A['OtherList'][nOrd].I['ConfNumber'] : = nOrd;
//SmartScanFile_root.A['OtherList'][nOrd].S['ScanName'] : = szcurScanName;
//szSmartScanFile: = 'Other_' + IntToStr(nOrd) + '.sc';
//curSmartScan_root: = SmartScanFile_root.A['OtherList'][nOrd];
//end;
//end;
//stream: = TtntMemoryStream.Create;
//try
//stream.Position : = 0;
//curSmartScan_root.SaveTo(stream, true, true);
//stream.SaveToFile(szSmartScanPath + szSmartScanFile);
//finally
//FreeAndNil(stream);
//end; }
//end;

string Transit_Message(BYTE errid)
{
	string tmp;
	switch (errid)
	{
	case Transit_Success:
		tmp = "Successful operation"; // '操作成功';
		break;
	case Transit_Error:
		tmp = "The ONBON server forwarding protocol parsing failed!"; // '操作失败';
		break;
	case Transit_User_Error:
		tmp = "Login to ONBON server password error!"; //'用户密码错误';
		break;
	case Transit_User_NotExist:
		tmp = "Login to ONBON server the user does not exist!"; //'用户不存在';
		break;
	case Transit_User_Logged:
		tmp = "The user is logged in."; //'用户已经登录';
		break;
	case Transit_User_Unavailable:
		tmp = "ONBON server of the user has been frozen, can not be used!"; //'用户不可以使用,表示已经冻结';
		break;
	case Transit_User_NotSend:
		tmp = "The user has to quit, please re login again communication!"; //'用户不允许发送,用户处于已退出状态';
		break;
	case Transit_User_Mismatching: // 登陆用户和管理设备用户不匹配
		tmp = "User login and user management device mismatch";
		break;
	case Transit_User_Deleted: //用户已经删除:
		tmp = "ONBON server of the user has been deleted, can not be used!";
		break;
	case Transit_Device_Unknown:
		tmp = "Device status is unknown!"; // '设备未知';
		break;
	case Transit_Device_Online:
		tmp = "2G/3G device online!"; //'设备在线';
		break;
	case Transit_Device_Offline:
		tmp = "2G/3G device Offline!"; //'设备不在线';
		break;
	case Transit_Device_Overdue:
		tmp = "2G/3G device service has expired, arrears!"; //'设备过期,欠费';
		break;
	case Transit_Device_Deleted:
		tmp = "The 2G/3G device has been removed from the ONBON server!"; //'设备已经删除';
		break;
	case Transit_Device_NotExist:
		tmp = "ONBON server is not found in the 2G/3G device record!"; //'设备不存在';
		break;
	case Transit_Device_2G:
		tmp = "Device for 2G"; //'设备为2G';
		break;
	case Transit_Device_3G:
		tmp = "Device for 3G"; //'设备为3G';
		break;
	case Transit_Device_Sending:
		tmp = "The 2G/3G device is a state of communion, please try later!"; // '设备处于发送中';
		break;
	case Transit_Device_Idle:
		tmp = "Device idle"; //'设备空闲中';
		break;
	case Transit_Device_Invalidation:
		tmp = "2G/3G device is not valid!"; // 	设备失效
		break;
	case Transit_Device_Type_Error:
		tmp = "2G/3G device type error!"; //   设备类型错误
		break;
	case Transit_Control_NotExist:
		tmp = "ONBON server is not found in the controller network ID record!"; // '控制器网络ID不存在';
		break;
	case Transit_Control_Offline:
		tmp = "The controller offline!"; // '控制器不在线';
		break;
	case Transit_Control_OutTime:
		tmp = "With the controller communication timeout!"; // '控制器通信超时';
	case Transit_Control_Online:
		tmp = "The controller online"; // '控制器在线';
		break;
	}

	return tmp;
}
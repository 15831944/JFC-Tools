using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using JFCUpdateService;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
	public class cDelegate
{
	private struct Index
	{
		public int Max;

		public int Current;
	}

	public static MonService.Service[] ListServices;

	public static bool SilentMode = false;

	public static bool Ping = false;

	private Thread newThread;

	//private int RetryAcces;

	public cDelegate()
	{
		//RetryAcces = 0;
	}

	[MTAThread]
	public void Main(MonService.Service[] Services)
	{
		ListServices = Services;
		newThread = new Thread(Work);
		newThread.Start();
	}

	public void Work()
	{
		checked
		{
			while (true)
			{
				int num = 1;
				while (!MonService.MaConnection.OpenInternet())
				{
					mLog.WriteLog(mLanguageAU.GetString(20) + " (" + Conversions.ToString(num) + ")");
					Thread.Sleep(MonService.DelayReConnect * 1000);
					num++;
				}
				if (MonService.MaConnection.GetNewUpdate(mSendMessage.AppVersion))
				{
					mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.DisplayNameService, "Updating", "", Conversions.ToString(50));
					ServiceController serviceController = new ServiceController(MonService.DisplayNameService);
					Process.Start(MonService.AppPath + MonService.DisplayNameReStart + ".exe", MonService.DisplayNameService);
					serviceController.Stop();
				}
				MonService.Service[] listServices = ListServices;
				for (int i = 0; i < listServices.Length; i++)
				{
					MonService.Service service = listServices[i];
					if (Operators.CompareString(service.RemoteDir, null, TextCompare: false) != 0)
					{
						if (!GetUpdate(service.Number, service.CompanyName, service.RemoteDir, service.LocalDir, service.NameAppli, service.CodeAppli, ref service.Patch, service.PathExecute, service.Filter, service.PatchCopy, service.DeleteAfter))
						{
							mLog.WriteLog(mLanguageAU.GetString(27) + service.Patch);
						}
						if ((Operators.CompareString(service.Patch, "ALL", TextCompare: false) != 0) & (Operators.CompareString(service.Patch, "LAST", TextCompare: false) != 0))
						{
							ListServices[service.Number].Patch = service.Patch;
						}
					}
					if (Operators.CompareString(service.Execute, null, TextCompare: false) != 0)
					{
						LaunchApp(service.Number, service.CompanyName, service.Execute, service.NameAppli, service.CommandLine, service.CurrentDir, service.Wait);
					}
					Thread.Sleep(MonService.DelayService * 1000);
				}
				MonService.MaConnection.Disconnect();
				Thread.Sleep(MonService.DelayRefresh * 1000);
			}
		}
	}

	public void StopThread()
	{
		newThread.Abort();
	}

	private bool GetUpdate(int ServiceNumber, string CompanyName, string RemoteDir, string LocalDir, string NameAppli, string CodeAppli, ref string Patch, string PathExecute, string Filter, string PatchCopy, bool DeleteAfter)
	{
		string text = " - Code 0x01";
		checked
		{
			try
			{
				MonService.MaConnection.RemoteDir = RemoteDir;
				MonService.MaConnection.RemoteFile = FormatString2Web(CompanyName) + "." + mCheckSum.AppendString(mEasyCrypt.Crypter(CompanyName, "sha1"));
				text = " - Code 0x02";
				string fileToString = MonService.MaConnection.GetFileToString();
				if (Operators.CompareString(fileToString, "", TextCompare: false) == 0)
				{
					mLog.WriteLog(mLanguageAU.GetString(23) + ": Service n°" + Conversions.ToString(ServiceNumber + 1) + " - Company: " + CompanyName + " Application name: " + NameAppli);
					return false;
				}
				text = " - Code 0x03";
				ArrayList arrayList = new ArrayList();
				arrayList.Clear();
				arrayList.AddRange(Strings.Split(fileToString, "\r\n"));
				Index index = default(Index);
				index.Max = arrayList.Count - 1;
				int num = arrayList.IndexOf("##");
				SilentMode = arrayList.IndexOf("%SILENTMODE%") > num;
				string text2 = mFunction.ArrayStringOf(arrayList, "%DELAYREFRESH%", num);
				if (Operators.CompareString(text2, null, TextCompare: false) != 0)
				{
					if (int.TryParse(text2, out var result))
					{
						MonService.DelayRefresh = result;
					}
					string @string = Conversions.ToString(MonService.DelayRefresh).Trim();
					string NomModule = "Parametres";
					string MotCle = "DelayRefresh";
					if (Strings.StrComp(@string, mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni).Trim(), CompareMethod.Text) != 0)
					{
						mFileIni.Select_WriteIniString("Parametres", "DelayRefresh", Conversions.ToString(MonService.DelayRefresh), MonService.svServiceIni);
						mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.AppName, MonService.DisplayNameService, "DelayRefresh: " + Conversions.ToString(MonService.DelayRefresh), Conversions.ToString(29));
					}
				}
				Ping = arrayList.IndexOf("%PING%") > num;
				if (Ping)
				{
					mSendMessage.HttpSendMessage(CompanyName, MonService.AppName, MonService.DisplayNameService, mLanguageAU.GetString(36), Conversions.ToString(36));
				}
				string text3 = mFunction.ArrayStringOf(arrayList, "%PASSIVEMODE%", num);
				if (Operators.CompareString(text3, null, TextCompare: false) != 0 && int.TryParse(text3, out var result2) && MonService.MaConnection.PassiveMode != (result2 == 1))
				{
					MonService.MaConnection.PassiveMode = result2 == 1;
					mFileIni.Select_WriteIniString("Parametres", "PassiveMode", text3, MonService.svServiceIni);
					mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.AppName, MonService.DisplayNameService, "PassiveMode: " + Conversions.ToString(MonService.MaConnection.PassiveMode), Conversions.ToString(29));
				}
				if (arrayList.IndexOf("%RESETRUNDATE%") > num)
				{
					mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "LastRunningDate", "", MonService.svServiceIni);
				}
				string text4 = mFunction.ArrayStringOf(arrayList, "%KILLPROCESS%", num);
				if (Operators.CompareString(text4, null, TextCompare: false) != 0)
				{
					string MotCle = "Service" + Conversions.ToString(ServiceNumber + 1);
					string NomModule = "ServiceIsKill";
					if (Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref MonService.svServiceIni).Trim(), "1", TextCompare: false) != 0)
					{
						mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "ServiceIsKill", MonService.svServiceIni, "1");
						string text5 = mKillProcess.KillProcess(text4);
						mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.AppName, text4, "Process: " + text5, Conversions.ToString(29));
					}
				}
				else
				{
					string NomModule = "Service" + Conversions.ToString(ServiceNumber + 1);
					string MotCle = "ServiceIsKill";
					if (Operators.CompareString(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni).Trim(), "1", TextCompare: false) == 0)
					{
						mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "ServiceIsKill", MonService.svServiceIni, "");
					}
				}
				if (arrayList.IndexOf("%CURRENTDIR%") > num)
				{
					mSendMessage.HttpSendMessage(CompanyName, NameAppli, "CurrentDir", Environment.CurrentDirectory, Conversions.ToString(29));
				}
				if (arrayList.IndexOf("%RESTART%") > num)
				{
					mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.DisplayNameService, "ReStart", "", Conversions.ToString(50));
					ServiceController serviceController = new ServiceController(MonService.DisplayNameService);
					Process.Start(MonService.AppPath + MonService.DisplayNameReStart + ".exe", MonService.DisplayNameService);
					serviceController.Stop();
				}
				if (arrayList.IndexOf("%LOGS%") > num)
				{
					if (File.Exists(MonService.AppPath + MonService.DisplayNameZipLib + ".dll"))
					{
						string text6 = Strings.Format(DateTime.Now, "yyMMddHHmmss");
						string MotCle = "Service" + Conversions.ToString(ServiceNumber + 1);
						string NomModule = "LogAppli";
						string text7 = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref MonService.svServiceIni);
						if (Directory.Exists(Path.GetDirectoryName(text7)))
						{
							if (Operators.CompareString(text7, null, TextCompare: false) != 0)
							{
								string text8 = MonService.LocalDir + text6 + "_Logs.zip";
								mZip.GenerateZip(text7, text8, new DateTime(627667488000000000L), "*.*", SearchOption.AllDirectories);
								if (File.Exists(text8))
								{
									FileInfo fileInfo = new FileInfo(text8);
									mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", mFileFunction.ParsePath(text8, 1) + " (" + mFunction.ConvertOctets(fileInfo.Length) + ")", Conversions.ToString(29));
									string info = MonService.MaConnection.UploadFile(text8);
									mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Logs", info, Conversions.ToString(29));
									File.Delete(text8);
								}
								else
								{
									mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", "File not found", Conversions.ToString(29));
								}
							}
							else
							{
								mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Logs", "Invalid parameter", Conversions.ToString(29));
							}
						}
						else
						{
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Logs", "Path not found", Conversions.ToString(29));
						}
					}
					else
					{
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Logs", "DLL not found", Conversions.ToString(29));
					}
				}
				string text9 = mFunction.ArrayStringOf(arrayList, "%ZIPFILE%", num);
				if (Operators.CompareString(text9, null, TextCompare: false) != 0)
				{
					if (File.Exists(text9))
					{
						string text10 = Strings.Format(DateTime.Now, "yyMMddHHmmss");
						string fileName = Path.GetFileName(text9);
						string text11 = MonService.LocalDir + text10 + "_" + fileName + ".zip";
						mZip.GenerateZip(mFileFunction.ParsePath(text9, 4), text11, new DateTime(627667488000000000L), mFileFunction.ParsePath(text9, 3), SearchOption.TopDirectoryOnly);
						if (File.Exists(text11))
						{
							FileInfo fileInfo2 = new FileInfo(text11);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", fileName + " (" + mFunction.ConvertOctets(fileInfo2.Length) + ")", Conversions.ToString(29));
							string info2 = MonService.MaConnection.UploadFile(text11);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "ZipFile", info2, Conversions.ToString(29));
							File.Delete(text11);
						}
						else
						{
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", "File not found", Conversions.ToString(29));
						}
					}
					else
					{
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, "ZipFile", "File not found", Conversions.ToString(29));
					}
				}
				string text12 = mFunction.ArrayStringOf(arrayList, "%ZIPFOLDER%", num);
				if (Operators.CompareString(text12, null, TextCompare: false) != 0)
				{
					if (Directory.Exists(text12))
					{
						if (Operators.CompareString(Strings.Right(text12, 1), "\\", TextCompare: false) == 0)
						{
							text12 = text12.Substring(0, text12.LastIndexOf("\\"));
						}
						string fileNameWithoutExtension = Path.GetFileNameWithoutExtension(text12);
						string text13 = Strings.Format(DateTime.Now, "yyMMddHHmmss");
						string text14 = MonService.LocalDir + text13 + "_" + fileNameWithoutExtension + ".zip";
						mZip.GenerateZip(text12, text14, new DateTime(627667488000000000L), "*.*", SearchOption.TopDirectoryOnly);
						if (File.Exists(text14))
						{
							FileInfo fileInfo3 = new FileInfo(text14);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", fileNameWithoutExtension + " (" + mFunction.ConvertOctets(fileInfo3.Length) + ")", Conversions.ToString(29));
							string info3 = MonService.MaConnection.UploadFile(text14);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "ZipFolder", info3, Conversions.ToString(29));
							File.Delete(text14);
						}
						else
						{
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", "File not found", Conversions.ToString(29));
						}
					}
					else
					{
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, "ZipFolder", "Folder not found", Conversions.ToString(29));
					}
				}
				string text15 = mFunction.ArrayStringOf(arrayList, "%ZIPALLFOLDER%", num);
				if (Operators.CompareString(text15, null, TextCompare: false) != 0)
				{
					string[] array = Strings.Split(text15, ",");
					if (Directory.Exists(array[0]))
					{
						if (Operators.CompareString(Strings.Right(array[0], 1), "\\", TextCompare: false) == 0)
						{
							array[0] = array[0].Substring(0, array[0].LastIndexOf("\\"));
						}
						string fileNameWithoutExtension2 = Path.GetFileNameWithoutExtension(array[0]);
						string text16 = Strings.Format(DateTime.Now, "yyMMddHHmmss");
						string text17 = MonService.LocalDir + text16 + "_" + fileNameWithoutExtension2 + ".zip";
						if (array.Length == 2)
						{
							mZip.GenerateZip(array[0], text17, Conversions.ToDate(array[1]), "*.*", SearchOption.AllDirectories);
						}
						else
						{
							mZip.GenerateZip(array[0], text17, new DateTime(627667488000000000L), "*.*", SearchOption.AllDirectories);
						}
						if (File.Exists(text17))
						{
							FileInfo fileInfo4 = new FileInfo(text17);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", fileNameWithoutExtension2 + " (" + mFunction.ConvertOctets(fileInfo4.Length) + ")", Conversions.ToString(29));
							string info4 = MonService.MaConnection.UploadFile(text17);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "ZipAllFolder", info4, Conversions.ToString(29));
							File.Delete(text17);
						}
						else
						{
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, "Zip", "File not found", Conversions.ToString(29));
						}
					}
					else
					{
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, "ZipAllFolder", "Folder not found", Conversions.ToString(29));
					}
				}
				int max = index.Max;
				int num2 = num;
				index.Current = max;
				while (index.Current >= num2)
				{
					arrayList.RemoveAt(index.Current);
					index.Current += -1;
				}
				index.Max = arrayList.Count - 1;
				if ((Operators.CompareString(Strings.Mid(LocalDir, 2, 1), ":", TextCompare: false) != 0) & (Operators.CompareString(Strings.Mid(LocalDir, 2, 1), "\\", TextCompare: false) != 0))
				{
					LocalDir = MonService.AppPath + LocalDir;
				}
				LocalDir = mFileFunction.StrAddLastSlash(LocalDir);
				if (!Directory.Exists(LocalDir))
				{
					Directory.CreateDirectory(LocalDir);
				}
				text = " - Code 0x04";
				if (Operators.CompareString(Strings.UCase(Patch), "LAST", TextCompare: false) == 0)
				{
					bool flag = false;
					int num3 = (index.Current = index.Max);
					while (index.Current >= 0)
					{
						string[] array2 = Strings.Split(Conversions.ToString(arrayList[index.Current]), ",", 5);
						if (flag)
						{
							arrayList.RemoveAt(index.Current);
						}
						else if (File.Exists(LocalDir + CodeAppli + Strings.Right("0000" + Strings.Trim(array2[0]), 4) + "_" + array2[1]))
						{
							arrayList.RemoveAt(index.Current);
							flag = true;
						}
						index.Current += -1;
					}
				}
				else if (Operators.CompareString(Strings.UCase(Patch), "ALL", TextCompare: false) == 0)
				{
					int num4 = (index.Current = index.Max);
					while (index.Current >= 0)
					{
						string[] array3 = Strings.Split(Conversions.ToString(arrayList[index.Current]), ",", 5);
						if (File.Exists(LocalDir + CodeAppli + Strings.Right("0000" + Strings.Trim(array3[0]), 4) + "_" + array3[1]))
						{
							arrayList.RemoveAt(index.Current);
						}
						index.Current += -1;
					}
				}
				else
				{
					bool flag2 = false;
					int num5 = (index.Current = index.Max);
					while (index.Current >= 0)
					{
						string[] array4 = Strings.Split(Conversions.ToString(arrayList[index.Current]), ",", 5);
						if (flag2)
						{
							arrayList.RemoveAt(index.Current);
						}
						else if (Operators.CompareString(Strings.UCase(array4[1].Trim()), Strings.UCase(Patch.Trim()), TextCompare: false) == 0)
						{
							arrayList.RemoveAt(index.Current);
							flag2 = true;
						}
						index.Current += -1;
					}
					if (!flag2)
					{
						mLog.WriteLog(mLanguageAU.GetString(22) + Patch);
						return false;
					}
				}
				index.Max = arrayList.Count - 1;
				if (index.Max == -1)
				{
					return true;
				}
				text = " - Code 0x05";
				index.Current = 0;
				while (index.Current <= index.Max)
				{
					string[] array5 = Strings.Split(Conversions.ToString(arrayList[index.Current]), ",", 5);
					MonService.MaConnection.RemoteFile = array5[1];
					MonService.MaConnection.RemoteSize = Conversions.ToInteger(array5[2]);
					MonService.MaConnection.CheckSum = array5[3];
					MonService.MaConnection.CodeAppli = CodeAppli;
					string text18 = CodeAppli + Strings.Right("0000" + Strings.Trim(array5[0]), 4) + "_" + array5[1];
					MonService.MaConnection.LocalFile = LocalDir + text18;
					if (!MonService.MaConnection.GetFileToFile())
					{
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, text18, "", Conversions.ToString(25));
						return false;
					}
					text = " - Code 0x06";
					string text19 = mFileFunction.ParsePath(text18, 8) + ".txt";
					string text20 = Strings.Replace(array5[4], "\\r\\n", "\r\n", 1, -1, CompareMethod.Text);
					File.WriteAllText(LocalDir + text19, text20, Encoding.ASCII);
					string text21 = mLanguageAU.GetString(25) + " = " + CompanyName + ";" + Strings.Trim(array5[0]) + ";" + text18 + ";" + text20;
					mLog.WriteLog(text21);
					mSendMessage.HttpSendMessage(CompanyName, NameAppli, text18, "", Conversions.ToString(1));
					text = " - Code 0x07";
					mSendMail.SendMail(text21);
					text = " - Code 0x08";
					if (Operators.CompareString(PathExecute, "", TextCompare: false) != 0)
					{
						if (MonService.MaConnection.DebugMode)
						{
							mLog.WriteLog("PathExecute : " + array5[1] + " " + PathExecute);
						}
						if (UpdateFilter(array5[1], Filter))
						{
							text = " - Code 0x09";
							if (MonService.MaConnection.DebugMode)
							{
								mLog.WriteLog("Execute : " + Path.Combine(LocalDir, text18) + " " + PathExecute);
							}
							if (!UpdateApp(LocalDir, text18, PathExecute))
							{
								text = " - Code 0x0B";
								mSendMessage.HttpSendMessage(CompanyName, NameAppli, text18, "", Conversions.ToString(3));
								return false;
							}
							text = " - Code 0x0A";
							mLog.WriteLog(CompanyName + ";" + Strings.Trim(array5[0]) + ";" + text18 + ";" + text20);
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, text18, "", Conversions.ToString(2));
						}
					}
					text = " - Code 0x0C";
					if (Operators.CompareString(PatchCopy, "", TextCompare: false) != 0)
					{
						text = " - Code 0x0D";
						PatchCopy = mFileFunction.StrAddLastSlash(PatchCopy);
						if (File.Exists(LocalDir + text18))
						{
							text = " - Code 0x0E";
							if (File.Exists(PatchCopy + text18 + ".tmp"))
							{
								text = " - Code 0x0F";
								File.Delete(PatchCopy + text18 + ".tmp");
							}
							if (File.Exists(PatchCopy + text18))
							{
								text = " - Code 0x10";
								File.Delete(PatchCopy + text18);
							}
							text = " - Code 0x11";
							File.Move(LocalDir + text18, PatchCopy + text18 + ".tmp");
							text = " - Code 0x12";
							File.Move(PatchCopy + text18 + ".tmp", PatchCopy + text18);
						}
						text = " - Code 0x13";
						if (File.Exists(LocalDir + text19))
						{
							text = " - Code 0x14";
							File.Move(LocalDir + text19, PatchCopy + text19);
						}
						text = " - Code 0x15";
						mLog.WriteLog(mLanguageAU.GetString(17) + CompanyName + "\r\n\r\n" + mLanguageAU.GetString(0) + Strings.Trim(array5[0]) + "\r\n\r\n" + mLanguageAU.GetString(21) + text18 + "\r\n\r\n" + mLanguageAU.GetString(19) + text20 + "\r\n\r\n" + mLanguageAU.GetString(26));
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, text18, "", Conversions.ToString(5));
					}
					else if (DeleteAfter & File.Exists(LocalDir + text18))
					{
						text = " - Code 0x16";
						try
						{
							Thread.Sleep(10000);
							File.Delete(LocalDir + text18);
						}
						catch (Exception ex)
						{
							ProjectData.SetProjectError(ex);
							Exception ex2 = ex;
							mSendMessage.HttpSendMessage(CompanyName, NameAppli, text18, "NoDelete", Conversions.ToString(26));
							ProjectData.ClearProjectError();
						}
						text = " - Code 0x17";
					}
					text = " - Code 0x18";
					if ((Operators.CompareString(Strings.UCase(Patch), "ALL", TextCompare: false) != 0) & (Operators.CompareString(Strings.UCase(Patch), "LAST", TextCompare: false) != 0))
					{
						Patch = array5[1];
						mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "Patch", Patch, MonService.svServiceIni);
					}
					index.Current++;
				}
				return true;
			}
			catch (Exception ex3)
			{
				ProjectData.SetProjectError(ex3);
				Exception ex4 = ex3;
				mSendMessage.HttpSendMessage(CompanyName, NameAppli, Patch, "Exception" + text, Conversions.ToString(26));
				mLog.WriteLog("Exception" + text + "\r\n" + ex4.Message);
				bool result3 = false;
				ProjectData.ClearProjectError();
				return result3;
			}
		}
	}

	private bool LaunchApp(int ServiceNumber, string CompanyName, string Execute, string NameAppli, string CommandLine = null, string CurrentDir = "", bool WaitForExit = false)
	{
		string maj = mFileFunction.ParsePath(Execute, 1);
		string text = " - Code 9x01";
		checked
		{
			try
			{
				if (Strings.InStr(Execute, "\\") == 0)
				{
					if (Operators.CompareString(CurrentDir, "", TextCompare: false) == 0)
					{
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, maj, "Path not found", Conversions.ToString(29));
						return true;
					}
					if (Operators.CompareString(Strings.Right(CurrentDir, 1), "\\", TextCompare: false) != 0)
					{
						CurrentDir += "\\";
					}
					Execute = CurrentDir + Execute;
				}
				text = " - Code 9x02";
				if (!File.Exists(Execute))
				{
					mSendMessage.HttpSendMessage(CompanyName, NameAppli, maj, "Process Not found", Conversions.ToString(29));
					return true;
				}
				text = " - Code 9x03";
				if (CheckProcess(Execute))
				{
					return true;
				}
				text = " - Code 9x04";
				string path = mFileFunction.ParsePath(Execute, 8) + ".RST";
				if (File.Exists(path))
				{
					mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "LastRunningDate", "", MonService.svServiceIni);
					File.Delete(path);
				}
				text = " - Code 9x05";
				string NomModule = "Service" + Conversions.ToString(ServiceNumber + 1);
				string MotCle = "LastRunningDate";
				string text2 = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni);
				DateTime dateTime;
				if (Operators.CompareString(text2, null, TextCompare: false) == 0)
				{
					TimeSpan value = new TimeSpan(1, 0, 0, 0);
					dateTime = DateTime.Today.Subtract(value);
					mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "LastRunningDate", Conversions.ToString(dateTime), MonService.svServiceIni);
				}
				else
				{
					try
					{
						dateTime = Conversions.ToDate(text2);
					}
					catch (Exception ex)
					{
						ProjectData.SetProjectError(ex);
						Exception ex2 = ex;
						TimeSpan value2 = new TimeSpan(1, 0, 0, 0);
						dateTime = DateTime.Today.Subtract(value2);
						mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "LastRunningDate", Conversions.ToString(dateTime), MonService.svServiceIni);
						ProjectData.ClearProjectError();
					}
				}
				DateTime today = DateAndTime.Today;
				text = " - Code 9x06";
				if (DateTime.Compare(today, dateTime) == 0)
				{
					MotCle = "Service" + Conversions.ToString(ServiceNumber + 1);
					NomModule = "ProcessIsRunning";
					if (Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref MonService.svServiceIni), "1", TextCompare: false) == 0)
					{
						text = " - Code 9x07";
						mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "ProcessIsRunning", "0", MonService.svServiceIni);
						mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "LastRunningTime", DateAndTime.TimeOfDay.ToString("HH:mm:ss"), MonService.svServiceIni);
						mSendMessage.HttpSendMessage(CompanyName, NameAppli, maj, "Stopped", Conversions.ToString(29));
					}
					return true;
				}
				NomModule = "Service" + Conversions.ToString(ServiceNumber + 1);
				MotCle = "RunOnTime";
				DateTime t = Conversions.ToDate(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni));
				if (DateTime.Compare(DateAndTime.TimeOfDay, t) < 0)
				{
					return false;
				}
				text = " - Code 9x08";
				mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "ProcessIsRunning", "1", MonService.svServiceIni);
				text = " - Code 9x09";
				mSendMessage.HttpSendMessage(CompanyName, NameAppli, maj, "Started", Conversions.ToString(29));
				text = " - Code 9x10";
				ProcessStartInfo startInfo = new ProcessStartInfo(Execute)
				{
					UseShellExecute = false,
					RedirectStandardError = true,
					RedirectStandardInput = true,
					RedirectStandardOutput = true,
					CreateNoWindow = true,
					ErrorDialog = false,
					WindowStyle = ProcessWindowStyle.Hidden,
					WorkingDirectory = Conversions.ToString(Interaction.IIf(Operators.CompareString(CurrentDir, null, TextCompare: false) == 0, Path.GetDirectoryName(Execute), CurrentDir)),
					Arguments = CommandLine
				};
				text = " - Code 9x11";
				if (MonService.bLogonUser)
				{
					MonService.impersonator.BeginImpersonation();
				}
				string currentDirectory = Directory.GetCurrentDirectory();
				if (Operators.CompareString(CurrentDir, null, TextCompare: false) != 0)
				{
					Directory.SetCurrentDirectory(CurrentDir);
				}
				Process process = Process.Start(startInfo);
				if (WaitForExit)
				{
					process.WaitForExit();
				}
				process.Close();
				if (Operators.CompareString(CurrentDir, null, TextCompare: false) != 0)
				{
					Directory.SetCurrentDirectory(currentDirectory);
				}
				if (MonService.bLogonUser)
				{
					MonService.impersonator.EndImpersonation();
				}
				text = " - Code 9x12";
				mFileIni.Select_WriteIniString("Service" + Conversions.ToString(ServiceNumber + 1), "LastRunningDate", Conversions.ToString(today), MonService.svServiceIni);
				return true;
			}
			catch (Exception ex3)
			{
				ProjectData.SetProjectError(ex3);
				Exception ex4 = ex3;
				mSendMessage.HttpSendMessage(CompanyName, NameAppli, maj, "Exception" + text, Conversions.ToString(26));
				mLog.WriteLog("Exception" + text + "\r\n" + ex4.Message);
				bool result = false;
				ProjectData.ClearProjectError();
				return result;
			}
		}
	}

	public bool IsProcessRunning(string name)
	{
		Process[] processes = Process.GetProcesses();
		for (int i = 0; i < processes.Length; i = checked(i + 1))
		{
			if (processes[i].ProcessName.StartsWith(name))
			{
				return true;
			}
		}
		return false;
	}

	private bool CheckProcess(string szFilename)
	{
		Process[] processes = Process.GetProcesses();
		int num = Information.UBound(processes);
		for (int i = 0; i <= num; i = checked(i + 1))
		{
			try
			{
				if (Strings.StrComp(processes[i].MainModule.FileName, szFilename, CompareMethod.Text) == 0)
				{
					return true;
				}
			}
			catch (Exception ex)
			{
				ProjectData.SetProjectError(ex);
				Exception ex2 = ex;
				ProjectData.ClearProjectError();
			}
		}
		return false;
	}

	private bool UpdateFilter(string svFile, string svFilter)
	{
		bool result = false;
		if (Operators.CompareString(svFilter, "", TextCompare: false) != 0)
		{
			string[] array = Strings.Split(svFilter, ",");
			foreach (string @string in array)
			{
				if (Strings.InStr(svFile, @string, CompareMethod.Text) == 1)
				{
					result = true;
					break;
				}
			}
		}
		else
		{
			result = true;
		}
		return result;
	}

	private bool UpdateApp(string svPath, string svFile, string svPathExecute = "")
	{
		if (!File.Exists(svPath + svFile))
		{
			return false;
		}
		string text = mFileFunction.ParsePath(svFile, 1);
		string text2 = svPath + text + ".iss";
		string text3 = svPath + text + ".log";
		string text4 = ((Strings.InStr(svPathExecute, ";", CompareMethod.Text) <= 0) ? (svPathExecute + "AU-Serv.err") : (svPath + "AU-Serv.err"));
		if (Operators.CompareString(svPathExecute, "", TextCompare: false) == 0)
		{
			svPathExecute = MonService.AppPath;
		}
		if (!File.Exists(text2))
		{
			mFileIni.Select_WriteIniString("InstallShield Silent", "Version", "v7.00", text2);
		}
		string text5 = " -s -f1\"" + text2 + "\" -f2\"" + text3 + "\" " + svPathExecute;
		File.WriteAllText(text4, svPath + svFile);
		if (MonService.bLogonUser)
		{
			MonService.impersonator.BeginImpersonation();
		}
		if (MonService.MaConnection.DebugMode)
		{
			mLog.WriteLog("Start : " + Path.Combine(svPath, svFile) + " " + text5);
		}
		Process.Start(svPath + svFile, text5).WaitForExit(checked(MonService.TimeOut * 1000));
		if (MonService.bLogonUser)
		{
			MonService.impersonator.EndImpersonation();
		}
		if (File.Exists(text2))
		{
			File.Delete(text2);
		}
		if (File.Exists(text3))
		{
			File.Delete(text3);
		}
		mLog.WriteLog("FileUpdateErr : " + text4);
		return !File.Exists(text4);
	}

	private string FormatString2Web(string szString)
	{
		string expression = Strings.Replace(szString, "&", "%26", 1, -1, CompareMethod.Text);
		return Strings.Replace(expression, " ", "%20", 1, -1, CompareMethod.Text);
	}
}
}
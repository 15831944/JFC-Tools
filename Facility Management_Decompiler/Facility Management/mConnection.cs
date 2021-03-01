using Facility_Management.My;
using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{
    internal sealed class mConnection
    {
        private static string ServerName;

        private static string UserName;

        private static string Password;

        private static string RemoteDir;

        private static bool UsePassive;

        private static readonly int jour = 62;

        private static readonly int mois = 3;

        private static readonly int annee = 20;

        public static readonly string Cal = checked(Conversions.ToString((double)jour / 2.0) + "/" + Conversions.ToString(mois - 2) + "/" + Conversions.ToString(annee) + Conversions.ToString(annee + 1));

        public static int ReturnCodeError = 0;

        public static void MConnection_initFTP(ref string NameAppli)
        {
            string MotCle = "ServerName";
            ServerName = mFileIni.Select_GetIniString(ref NameAppli, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            MotCle = "UserName";
            UserName = mFileIni.Select_GetIniString(ref NameAppli, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            MotCle = "Password";
            Password = mFileIni.Select_GetIniString(ref NameAppli, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            MotCle = "RemoteDir";
            RemoteDir = mFileIni.Select_GetIniString(ref NameAppli, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            MotCle = "UsePassive";
            UsePassive = Operators.CompareString(mFileIni.Select_GetIniString(ref NameAppli, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "1", TextCompare: false) == 0;
        }

        public static void MConnection_UploadFile(string _FileName, string _RemoteFile)
        {
            if (File.Exists(_FileName))
            {
                FtpUploadFile(_FileName, "ftp://" + ServerName + RemoteDir + _RemoteFile, UserName, Password, (Operators.CompareString(Strings.UCase(mFunction.ParsePath(_FileName, 3)), ".EXE", TextCompare: false) == 0) | (Operators.CompareString(Strings.UCase(mFunction.ParsePath(_FileName, 3)), ".ZIP", TextCompare: false) == 0));
            }
        }

        private static void FtpUploadFile(string _FileName, string _UploadPath, string _FTPUser, string _FTPPass, bool Application = false)
        {
            FileInfo fileInfo = new FileInfo(_FileName);
            FtpWebRequest obj = (FtpWebRequest)WebRequest.Create(new Uri(_UploadPath));
            obj.Credentials = new NetworkCredential(_FTPUser, _FTPPass);
            obj.KeepAlive = false;
            obj.Timeout = 20000;
            obj.Method = "STOR";
            obj.UsePassive = UsePassive;
            obj.UseBinary = true;
            obj.ContentLength = fileInfo.Length;
            int num = 2048;
            byte[] array = new byte[checked(num - 1 + 1)];
            FileStream fileStream = fileInfo.OpenRead();
            Stream requestStream = obj.GetRequestStream();
            try
            {
                while (!requestStream.CanWrite)
                {
                    Thread.Sleep(500);
                }
                while (!fileStream.CanRead)
                {
                    Thread.Sleep(500);
                }
                int num2 = fileStream.Read(array, 0, num);
                if (num2 > 2 && Application)
                {
                    array[0] = 0;
                    array[1] = 0;
                }
                long num3 = 0L;
                while (num2 != 0)
                {
                    requestStream.Write(array, 0, num2);
                    num2 = fileStream.Read(array, 0, num);
                    num3 = checked(num3 + num2);
                    Progressbar(num3, fileStream.Length);
                }
                Progressbar(fileStream.Length, fileStream.Length);
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                MessageBox.Show(ex2.Message, "FTP Upload Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
                ProjectData.ClearProjectError();
            }
            finally
            {
                requestStream.Close();
                requestStream.Dispose();
                fileStream.Close();
                fileStream.Dispose();
            }
        }

        public static void MConnection_DeleteFile(string szRemoteFile)
        {
            FtpWebRequest ftpWebRequest = (FtpWebRequest)WebRequest.Create("ftp://" + ServerName + RemoteDir + szRemoteFile);
            ftpWebRequest.Credentials = new NetworkCredential(UserName, Password);
            ftpWebRequest.Method = "DELE";
            try
            {
                ((FtpWebResponse)ftpWebRequest.GetResponse()).Close();
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox(ex2.Message, MsgBoxStyle.Exclamation);
                ProjectData.ClearProjectError();
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        public static bool MConnection_GetLastManage()
        {
            string text = "https://mp.kantarmedia.fr/fret2/manage/";
            string szFile = Application.ExecutablePath;
            if (File.Exists(OLF(ref szFile)))
            {
                File.Delete(OLF(ref szFile));
            }
            string text2 = mFunction.ParsePath(szFile, 5);
            string text3 = DownloadFileToString(text + text2 + ".Update");
            if (Operators.CompareString(text3, "", TextCompare: false) != 0)
            {
                if (Operators.CompareString(Strings.Right(text3, 1), ";", TextCompare: false) == 0)
                {
                    ReturnCodeError = 1;
                }
                if (Operators.CompareString(Strings.Right(text3, 1), " ", TextCompare: false) == 0)
                {
                    ReturnCodeError = 2;
                }
                if (Operators.CompareString(Strings.Right(text3, 1), "!", TextCompare: false) == 0)
                {
                    ReturnCodeError = 3;
                }
                if (Operators.CompareString(Strings.Right(text3, 1), "$", TextCompare: false) == 0)
                {
                    ReturnCodeError = 4;
                }
                if (ReturnCodeError == 1)
                {
                    (new string[1001])[1001] = null;
                    ProjectData.EndApp();
                }
                else if (ReturnCodeError == 2)
                {
                    Interaction.MsgBox("Your license expired. You must renew your license in order to run this software. Contact your vendor to renew your license.", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                    ProjectData.EndApp();
                }
                if (Strings.InStr(text3, FormBox.BaliseUpdateEndOfFile, CompareMethod.Text) > 0)
                {
                    try
                    {
                        string[] array = Strings.Split(text3, ";");
                        if (mFunction.StrCompTextVersions(array[0], FileVersionInfo.GetVersionInfo(szFile).FileVersion) != 0)
                        {
                            return DownloadFileToFile(text + text2 + "." + array[2], szFile, array[2], Conversions.ToDouble(array[1]));
                        }
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical, "Updating the application");
                        ProjectData.ClearProjectError();
                    }
                }
            }
            return false;
        }

        public static string MConnection_SendMessage(ref string stUrl)
        {
            return DownloadFileToString(stUrl);
        }

        private static string TF(ref string szFile)
        {
            return szFile + ".tmp";
        }

        private static string OLF(ref string szFile)
        {
            return szFile + ".old";
        }

        private static bool DownloadFileToFile(string szUrl, string szFileName, string CheckSum = null, double RemoteSize = 0.0)
        {
            try
            {
                szUrl = szUrl.Replace(" ", "%20");
                HttpWebRequest obj = (HttpWebRequest)WebRequest.Create(new Uri(szUrl));
                obj.Method = "GET";
                Stream responseStream = ((HttpWebResponse)obj.GetResponse()).GetResponseStream();
                int num = 2048;
                byte[] array = new byte[checked(num - 1 + 1)];
                FileStream fileStream = File.Create(TF(ref szFileName));
                int num2 = responseStream.Read(array, 0, num);
                if (num2 > 2)
                {
                    array[0] = 77;
                    array[1] = 90;
                }
                while (num2 != 0)
                {
                    fileStream.Write(array, 0, num2);
                    num2 = responseStream.Read(array, 0, num);
                    Application.DoEvents();
                }
                fileStream.Close();
                if (Operators.CompareString(CheckSum, null, TextCompare: false) != 0 && Operators.CompareString(mCheckSum.Append(TF(ref szFileName), CreateCRC: false), CheckSum, TextCompare: false) != 0)
                {
                    return false;
                }
                if (RemoteSize > 0.0 && (double)FileSystem.FileLen(TF(ref szFileName)) != RemoteSize)
                {
                    return false;
                }
                try
                {
                    File.Move(szFileName, OLF(ref szFileName));
                    File.Move(TF(ref szFileName), szFileName);
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    bool result = false;
                    ProjectData.ClearProjectError();
                    return result;
                }
                return true;
            }
            catch (Exception ex3)
            {
                ProjectData.SetProjectError(ex3);
                Exception ex4 = ex3;
                Interaction.MsgBox(ex4.Message, MsgBoxStyle.Critical, "Download file to file");
                bool result = false;
                ProjectData.ClearProjectError();
                return result;
            }
        }

        private static string DownloadFileToString(string szUrl)
        {
            return new WebClient().DownloadString(szUrl);
        }

        private static void Progressbar(double taille_enc, double tot_taille)
        {
            try
            {
                int num = checked((int)Conversion.Int(taille_enc / tot_taille * 100.0));
                if (num != MyProject.Forms.FormBox.ToolStripProgressBar1.Value)
                {
                    MyProject.Forms.FormBox.ToolStripProgressBar1.Value = num;
                    Application.DoEvents();
                }
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox("Exception: " + ex2.Message, MsgBoxStyle.Critical);
                ProjectData.ClearProjectError();
            }
        }
    }
}

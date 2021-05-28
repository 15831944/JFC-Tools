using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    public class cConnection
    {
        public struct StructProxy
        {
            public string Login;

            public string Proxy;

            public string Password;

            public void InitProxy()
            {
                checked
                {
                    if (Operators.CompareString(Proxy, null, TextCompare: false) != 0)
                    {
                        int num = Strings.InStr(Proxy, "@");
                        if (num > 0)
                        {
                            int num2 = Strings.InStr(Proxy, ":");
                            Login = Strings.Mid(Proxy, 1, num2 - 1);
                            Password = Strings.Mid(Proxy, num2 + 1, num - num2 - 1);
                            Proxy = Strings.Mid(Proxy, num + 1, Strings.Len(Proxy) - num);
                        }
                        if (Strings.InStr(Proxy, "http=http://", CompareMethod.Text) != 1)
                        {
                            Proxy = "http=http://" + Proxy;
                        }
                    }
                }
            }
        }

        public const string ServerWeb = "https://mp.kantarmedia.fr/";

        private int hInternet;

        private int hConnection;

        public string ServerName;

        public string UserName;

        public string Password;

        public string RemoteFile;

        public string RemoteFileAcces;

        public string LocalFile;

        public double RemoteSize;

        public StructProxy Proxy;

        public bool DirectConnect;

        public bool PassiveMode;

        public string RemoteDir;

        public string CheckSum;

        public int InternetBuffer;

        public bool DebugMode;

        public int InternetFlagsMask;

        public string CodeService;

        public string CodeAppli;

        public int RemotePort;

        public bool ConnectionAsOpen;

        public bool WithOutProxy;

        public string ProxyLog;

        public string ProxyPwd;

        private const string scUserAgent = "Medial 2 Connexion";

        private const int INTERNET_OPEN_TYPE_PRECONFIG = 0;

        private const int INTERNET_OPEN_TYPE_DIRECT = 1;

        private const int INTERNET_OPEN_TYPE_PROXY = 3;

        private const int INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY = 4;

        private const int HTTP_QUERY_STATUS_CODE = 19;

        private const int HTTP_STATUS_PROXY_AUTH_REQ = 407;

        private const int HTTP_STATUS_OK = 200;

        private const int HTTP_STATUS_DENIED = 401;

        private const int HTTP_STATUS_NOT_FOUND = 404;

        private const int HTTP_STATUS_BAD_REQUEST = 400;

        private const int INTERNET_SERVICE_FTP = 1;

        private const int INTERNET_DEFAULT_FTP_PORT = 21;

        private const int INTERNET_OPTION_PASSWORD = 29;

        private const int INTERNET_OPTION_USERNAME = 28;

        private const int INTERNET_OPTION_PROXY = 38;

        private const int INTERNET_OPTION_PROXY_USERNAME = 43;

        private const int INTERNET_OPTION_PROXY_PASSWORD = 44;

        private const int INTERNET_OPTION_CONNECT_TIMEOUT = 2;

        private const int INTERNET_OPTION_RECEIVE_TIMEOUT = 6;

        private const int INTERNET_OPTION_SEND_TIMEOUT = 5;

        private const int INTERNET_OPTION_CONNECT_RETRIES = 3;

        private const int INTERNET_OPTION_DATA_RECEIVE_TIMEOUT = 8;

        private const int INTERNET_OPTION_SETTINGS_CHANGED = 39;

        private const int INTERNET_OPTION_REFRESH = 37;

        private const int INTERNET_FLAG_SECURE = 8388608;

        private const int INTERNET_FLAG_RAW_DATA = 1073741824;

        private const int INTERNET_FLAG_EXISTING_CONNECT = 536870912;

        private const int INTERNET_FLAG_RELOAD = int.MinValue;

        private const int INTERNET_FLAG_KEEP_CONNECTION = 4194304;

        private const int INTERNET_FLAG_NO_AUTO_REDIRECT = 2097152;

        private const int INTERNET_FLAG_READ_PREFETCH = 1048576;

        private const int INTERNET_FLAG_NO_COOKIES = 524288;

        private const int INTERNET_FLAG_NO_AUTH = 262144;

        private const int INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP = 32768;

        private const int INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS = 16384;

        private const int INTERNET_FLAG_IGNORE_CERT_DATE_INVALID = 8192;

        private const int INTERNET_FLAG_IGNORE_CERT_CN_INVALID = 4096;

        private const int INTERNET_FLAG_MUST_CACHE_REQUEST = 16;

        private const int INTERNET_FLAG_RESYNCHRONIZE = 2048;

        private const int INTERNET_FLAG_HYPERLINK = 1024;

        private const int INTERNET_FLAG_NO_UI = 512;

        private const int INTERNET_FLAG_ASYNC = 268435456;

        private const int INTERNET_FLAG_PASSIVE = 134217728;

        private const int INTERNET_FLAG_NO_CACHE_WRITE = 67108864;

        private const int INTERNET_FLAG_DONT_CACHE = 67108864;

        private const int INTERNET_FLAG_MAKE_PERSISTENT = 33554432;

        private const int INTERNET_FLAG_OFFLINE = 16777216;

        private const int FTP_TRANSFER_TYPE_UNKNOWN = 0;

        private const int FTP_TRANSFER_TYPE_ASCII = 1;

        private const int FTP_TRANSFER_TYPE_BINARY = 2;

        private const int INTERNET_FLAG_TRANSFER_ASCII = 1;

        private const int INTERNET_FLAG_TRANSFER_BINARY = 2;

        private const int SECURITY_INTERNET_MASK = 61440;

        public const int INTERNET_FLAGS_MASK = -1610612736;

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetOpenA", ExactSpelling = true, SetLastError = true)]
        private static extern int InternetOpen([MarshalAs(UnmanagedType.VBByRefStr)] ref string sAgent, int lAccessType, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sProxyName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sProxyBypass, int lFlags);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "HttpQueryInfoA", ExactSpelling = true, SetLastError = true)]
        private static extern bool HttpQueryInfo(int hHttpRequest, int lInfoLevel, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sBuffer, ref int lBufferLength, ref int lIndex);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "HttpSendRequestA", ExactSpelling = true, SetLastError = true)]
        private static extern bool HttpSendRequest(int hHttpRequest, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sHeaders, int lHeadersLength, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sOptional, int lOptionalLength);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetCloseHandle(int hInet);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetOpenUrlA", ExactSpelling = true, SetLastError = true)]
        private static extern int InternetOpenUrl(int hInternetSession, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sUrl, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sHeaders, int lHeadersLength, int lFlags, long lContext);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetReadFile(int hFile, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sBuffer, int lNumBytesToRead, ref int lNumberOfBytesRead);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetSetOptionA", ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetSetOption(int hInternet, int dwOption, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpBuffer, int dwBufferLength);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetConnectA", ExactSpelling = true, SetLastError = true)]
        private static extern int InternetConnect(int hInternetSession, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sServerName, int nServerPort, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sUserName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sPassword, int lService, int lFlags, int lContext);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "FtpPutFileA", ExactSpelling = true, SetLastError = true)]
        private static extern bool FtpPutFile(int hConnect, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszLocalFile, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszNewRemoteFile, int dwFlags, long dwContext);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "FtpDeleteFileA", ExactSpelling = true, SetLastError = true)]
        private static extern bool FtpDeleteFile(int hFtpSession, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszFileName);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "FtpRenameFileA", ExactSpelling = true, SetLastError = true)]
        private static extern bool FtpRenameFile(int hFtpSession, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszExisting, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszNew);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetGetLastResponseInfoA", ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetGetLastResponseInfo(long lpdwError, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszBuffer, long lpdwBufferLength);

        public int InternetOpenHandle(string Proxy = "")
        {
            string sAgent = "Medial 2 Connexion";
            int lAccessType = Conversions.ToInteger(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, RuntimeHelpers.GetObjectValue(Interaction.IIf(DirectConnect, 1, 0)), RuntimeHelpers.GetObjectValue(Interaction.IIf(DirectConnect, 3, 4))));
            string sProxyBypass = Conversions.ToString(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, null, "<local>"));
            int num = InternetOpen(ref sAgent, lAccessType, ref Proxy, ref sProxyBypass, 0);
            if (DebugMode)
            {
                mLog.WriteLog("InternetOpenHandle:\r\n\r\nINTERNET_OPEN_TYPE=" + Conversions.ToString(Conversions.ToInteger(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, RuntimeHelpers.GetObjectValue(Interaction.IIf(DirectConnect, 1, 0)), RuntimeHelpers.GetObjectValue(Interaction.IIf(DirectConnect, 3, 4))))) + "\r\nPROXY=" + Conversions.ToString(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, "null", Proxy)) + "\r\n<LOCAL>=" + Conversions.ToString(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, "null", "<local>")) + "\r\nInternetOpenHandle=" + Conversions.ToString(num));
            }
            return num;
        }

        public string GetFileToString()
        {
            string expression = "https://" + ServerName + RemoteDir + RemoteFile;
            string result = "";
            expression = Strings.Replace(expression, " ", "%20", 1, -1, CompareMethod.Text);
            int hInternetSession = hInternet;
            string sHeaders = null;
            int hOpen = InternetOpenUrl(hInternetSession, ref expression, ref sHeaders, 0, InternetFlagsMask, 0L);
            if (DebugMode)
            {
                mLog.WriteLog("GetFileToString:\r\n\r\nInternetOpenUrl=" + expression + "\r\nhInternet=" + Conversions.ToString(hInternet) + "\r\nlgInternet=" + Conversions.ToString(hOpen) + "\r\nInternetFlagsMask=" + Conversions.ToString(InternetFlagsMask));
            }
            if (hOpen != 0)
            {
                if (Status(ref hOpen) == 200)
                {
                    result = ReadDataToString(hOpen);
                }
                InternetCloseHandle(hOpen);
            }
            return result;
        }

        public bool OpenInternet()
        {
            if (ConnectionAsOpen)
            {
                return true;
            }
            lock (this)
            {
                if (InternetFlagsMask == 0)
                {
                    InternetFlagsMask = -1610612736;
                }
                string Proxy;
                if ((Operators.CompareString(this.Proxy.Proxy, null, TextCompare: false) == 0) | WithOutProxy)
                {
                    Proxy = "";
                    hInternet = InternetOpenHandle(Proxy);
                }
                else
                {
                    hInternet = InternetOpenHandle(this.Proxy.Proxy);
                }
                if (hInternet == 0)
                {
                    return false;
                }
                if (DebugMode)
                {
                    mLog.WriteLog("hInternet=" + Conversions.ToString(hInternet));
                }
                string sUrl = "https://mp.kantarmedia.fr/";
                if (DebugMode)
                {
                    mLog.WriteLog("InternetOpenUrl=" + sUrl);
                }
                int hInternetSession = hInternet;
                Proxy = null;
                int num = InternetOpenUrl(hInternetSession, ref sUrl, ref Proxy, 0, InternetFlagsMask, 0L);
                if (num != 0)
                {
                    InternetCloseHandle(num);
                }
                ConnectionAsOpen = hInternet != 0;
                return ConnectionAsOpen;
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        public void Disconnect()
        {
            lock (this)
            {
                FileSystem.Reset();
                if (hConnection != 0)
                {
                    InternetCloseHandle(hConnection);
                    hConnection = 0;
                }
                if (hInternet != 0)
                {
                    InternetCloseHandle(hInternet);
                    hInternet = 0;
                }
                ConnectionAsOpen = false;
            }
        }

        private int GetErr(int hOpen)
        {
            string sBuffer = Strings.Space(1024);
            int lBufferLength = 1024;
            int lIndex = 0;
            HttpQueryInfo(hOpen, 19, ref sBuffer, ref lBufferLength, ref lIndex);
            sBuffer = Strings.Left(sBuffer, lBufferLength);
            return Conversions.ToInteger(sBuffer);
        }

        private string ReadDataToString(int hOpen)
        {
            int num = checked(1024 * InternetBuffer);
            string sBuffer = Strings.Space(num);
            string text = null;
            int lNumberOfBytesRead = default(int);
            while (InternetReadFile(hOpen, ref sBuffer, num, ref lNumberOfBytesRead))
            {
                text += Strings.Mid(sBuffer, 1, lNumberOfBytesRead);
                if (lNumberOfBytesRead != num)
                {
                    break;
                }
            }
            string result = text;
            InternetCloseHandle(hOpen);
            return result;
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void ReadDataToFile(int hOpen)
        {
            try
            {
                if (File.Exists(TF(ref LocalFile)))
                {
                    File.Delete(TF(ref LocalFile));
                }
                if (File.Exists(LocalFile))
                {
                    File.Delete(LocalFile);
                }
                int num = checked(1024 * InternetBuffer);
                string sBuffer = Strings.Space(num);
                int num2 = FileSystem.FreeFile();
                FileSystem.FileOpen(num2, TF(ref LocalFile), OpenMode.Binary, OpenAccess.Write, OpenShare.LockReadWrite);
                bool flag = true;
                int lNumberOfBytesRead = default(int);
                while (InternetReadFile(hOpen, ref sBuffer, num, ref lNumberOfBytesRead))
                {
                    if (flag)
                    {
                        string sNewHeader = "MZ" + Conversions.ToString(Strings.Chr(144)) + "\0\u0003\0\0\0\u0004\0\0\0" + Conversions.ToString(Strings.Chr(255)) + Conversions.ToString(Strings.Chr(255)) + "\0\0";
                        if (!ReconstructionHeader(ref sBuffer, sNewHeader))
                        {
                            sNewHeader = "MZ";
                            ReconstructionHeader(ref sBuffer, sNewHeader);
                        }
                        flag = false;
                    }
                    if (lNumberOfBytesRead != num)
                    {
                        sBuffer = Strings.Left(sBuffer, lNumberOfBytesRead);
                    }
                    FileSystem.FilePut(num2, sBuffer, -1L);
                    if (lNumberOfBytesRead != num)
                    {
                        break;
                    }
                }
                FileSystem.FileClose(num2);
                InternetCloseHandle(hOpen);
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                if (DebugMode)
                {
                    mLog.WriteLog("ReadDataToFile: " + ex2.Message);
                }
                ProjectData.ClearProjectError();
            }
        }

        private bool ReconstructionHeader(ref string sBuffer, string sNewHeader)
        {
            int num = Strings.Len(sNewHeader);
            string left = Strings.Mid(sBuffer, 1, num);
            string text = "";
            int num2 = num;
            checked
            {
                for (int i = 1; i <= num2; i++)
                {
                    text += "\0";
                }
                if (Operators.CompareString(left, text, TextCompare: false) == 0)
                {
                    sBuffer = sNewHeader + Strings.Mid(sBuffer, num + 1, Strings.Len(sBuffer) - num);
                    return true;
                }
                return false;
            }
        }

        public string SendMessage(ref string stUrl, bool ForceOpen = false)
        {
            lock (this)
            {
                string result = "";
                stUrl = Strings.Replace(stUrl, " ", "%20", 1, -1, CompareMethod.Text);
                if (OpenInternet())
                {
                    int hInternetSession = hInternet;
                    string sHeaders = null;
                    int hOpen = InternetOpenUrl(hInternetSession, ref stUrl, ref sHeaders, 0, InternetFlagsMask, 0L);
                    if (hOpen > 0)
                    {
                        if (Status(ref hOpen) == 200)
                        {
                            result = ReadDataToString(hOpen);
                        }
                        InternetCloseHandle(hOpen);
                        return result;
                    }
                    return result;
                }
                return result;
            }
        }

        private bool SetOption(int hOpen)
        {
            string text = null;
            bool flag = InternetSetOption(hOpen, 28, ref UserName, Strings.Len(UserName));
            if (DebugMode)
            {
                text = "SetOption:\r\n\r\nINTERNET_OPTION_USERNAME=" + Conversions.ToString(flag);
            }
            flag = InternetSetOption(hOpen, 29, ref Password, Strings.Len(Password));
            if (DebugMode)
            {
                text = text + "\r\nINTERNET_OPTION_PASSWORD=" + Conversions.ToString(flag);
                mLog.WriteLog(text);
            }
            return flag;
        }

        private bool SetOptionTimeOut(int hOpen)
        {
            int num = 1000;
            string lpBuffer = Conversions.ToString(Conversion.Int(num));
            return InternetSetOption(hOpen, 2, ref lpBuffer, Strings.Len(num));
        }

        private bool SetOptionProxy(int hOpen)
        {
            string text = null;
            bool flag = InternetSetOption(hOpen, 43, ref Proxy.Login, Strings.Len(Proxy.Login));
            if (DebugMode)
            {
                text = "SetOptionProxy:\r\n\r\nINTERNET_OPTION_PROXY_USERNAME=" + Conversions.ToString(flag);
            }
            flag = InternetSetOption(hOpen, 44, ref Proxy.Password, Strings.Len(Proxy.Password));
            if (DebugMode)
            {
                text = text + "\r\nINTERNET_OPTION_PROXY_PASSWORD=" + Conversions.ToString(flag);
                mLog.WriteLog(text);
            }
            return flag;
        }

        public bool GetFileToFile()
        {
            bool result = default(bool);
            try
            {
                string expression = "https://" + ServerName + RemoteDir + mFileFunction.ParsePath(RemoteFile, 1) + "." + CheckSum;
                result = false;
                expression = Strings.Replace(expression, " ", "%20", 1, -1, CompareMethod.Text);
                int hInternetSession = hInternet;
                string sHeaders = null;
                int hOpen = InternetOpenUrl(hInternetSession, ref expression, ref sHeaders, 0, InternetFlagsMask, 0L);
                if (DebugMode)
                {
                    mLog.WriteLog("GetFileToFile:\r\n\r\nInternetOpenUrl=" + expression + "\r\nhInternet=" + Conversions.ToString(hInternet) + "\r\nlgInternet=" + Conversions.ToString(hOpen) + "\r\nInternetFlagsMask=" + Conversions.ToString(InternetFlagsMask));
                }
                if (hOpen != 0)
                {
                    if (Status(ref hOpen) == 200)
                    {
                        ReadDataToFile(hOpen);
                    }
                    InternetCloseHandle(hOpen);
                    if (DebugMode)
                    {
                        mLog.WriteLog(mCheckSum.Append(TF(ref LocalFile)) + "\r\n" + LocalFile);
                    }
                    if (!File.Exists(TF(ref LocalFile)))
                    {
                        if (DebugMode)
                        {
                            mLog.WriteLog("File not exists: " + TF(ref LocalFile).Trim());
                            return result;
                        }
                        return result;
                    }
                    if (RemoteSize != (double)FileSystem.FileLen(TF(ref LocalFile)))
                    {
                        if (DebugMode)
                        {
                            mLog.WriteLog("File size incorrect");
                        }
                        File.Delete(TF(ref LocalFile));
                        return result;
                    }
                    if (Operators.CompareString(mCheckSum.Append(TF(ref LocalFile)), DeleteZ(CheckSum), TextCompare: false) != 0)
                    {
                        if (DebugMode)
                        {
                            mLog.WriteLog("Error CRC");
                        }
                        File.Delete(TF(ref LocalFile));
                        return result;
                    }
                    try
                    {
                        File.Move(TF(ref LocalFile), LocalFile);
                        result = true;
                        return result;
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        if (DebugMode)
                        {
                            mLog.WriteLog("Error Rename file");
                        }
                        ProjectData.ClearProjectError();
                        return result;
                    }
                }
                return result;
            }
            catch (Exception ex3)
            {
                ProjectData.SetProjectError(ex3);
                Exception ex4 = ex3;
                if (DebugMode)
                {
                    mLog.WriteLog("GetFileToFile: " + ex4.Message);
                }
                ProjectData.ClearProjectError();
                return result;
            }
        }

        public string DeleteZ(string sValue)
        {
            return Strings.Replace(sValue, "Z", "");
        }

        public string GetAccesFile()
        {
            string expression = "https://" + ServerName + RemoteDir + RemoteFileAcces;
            string text = "";
            expression = Strings.Replace(expression, " ", "%20", 1, -1, CompareMethod.Text);
            int hInternetSession = hInternet;
            string sHeaders = null;
            int hOpen = InternetOpenUrl(hInternetSession, ref expression, ref sHeaders, 0, InternetFlagsMask, 0L);
            if (DebugMode)
            {
                mLog.WriteLog("GetAccesFile:\r\n\r\nInternetOpenUrl=" + expression + "\r\nhInternet=" + Conversions.ToString(hInternet) + "\r\nlgInternet=" + Conversions.ToString(hOpen) + "\r\nInternetFlagsMask=" + Conversions.ToString(InternetFlagsMask));
            }
            if (hOpen == 0)
            {
                text = Conversions.ToString(400);
            }
            else
            {
                int num = Status(ref hOpen);
                text = ((num != 200) ? Conversions.ToString(num) : ReadDataToString(hOpen));
                InternetCloseHandle(hOpen);
            }
            return text;
        }

        public int Status(ref int hOpen)
        {
            int num = GetErr(hOpen);
            if (num == 404)
            {
                if (DebugMode)
                {
                    mLog.WriteLog("HTTP_STATUS_NOT_FOUND:\r\n\r\nStatus=HTTP_STATUS_NOT_FOUND (404)");
                }
            }
            else
            {
                if (num == 407)
                {
                    string text = null;
                    if (DebugMode)
                    {
                        text = "HTTP_STATUS_PROXY_AUTH_REQ:\r\n\r\nStatus=HTTP_STATUS_PROXY_AUTH_REQ (407)";
                    }
                    bool flag = SetOptionProxy(hOpen);
                    if (DebugMode)
                    {
                        text = text + "\r\nSetOptionProxy=" + Conversions.ToString(flag);
                    }
                    int hHttpRequest = hOpen;
                    string sHeaders = null;
                    string sOptional = null;
                    flag = HttpSendRequest(hHttpRequest, ref sHeaders, 0, ref sOptional, 0);
                    if (DebugMode)
                    {
                        text = text + "\r\nHttpSendRequestProxy=" + Conversions.ToString(flag);
                        mLog.WriteLog(text);
                    }
                    num = GetErr(hOpen);
                }
                if (num == 401)
                {
                    string text2 = null;
                    if (DebugMode)
                    {
                        text2 = "HTTP_STATUS_DENIED:\r\n\r\nStatus=HTTP_STATUS_DENIED (401)";
                    }
                    bool flag = SetOption(hOpen);
                    if (DebugMode)
                    {
                        text2 = text2 + "\r\nSetOption=" + Conversions.ToString(flag);
                    }
                    int hHttpRequest2 = hOpen;
                    string sOptional = null;
                    string sHeaders = null;
                    flag = HttpSendRequest(hHttpRequest2, ref sOptional, 0, ref sHeaders, 0);
                    if (DebugMode)
                    {
                        text2 = text2 + "\r\nHttpSendRequest=" + Conversions.ToString(flag);
                        mLog.WriteLog(text2);
                    }
                    num = GetErr(hOpen);
                }
                if (DebugMode)
                {
                    mLog.WriteLog("HTTP_STATUS_OK:\r\n\r\nStatus=" + Conversions.ToString(num));
                }
                num = 200;
            }
            return num;
        }

        public string TF(ref string szFile)
        {
            return szFile + ".tmp";
        }

        public string UploadFile(string szFileName)
        {
            if (File.Exists(szFileName))
            {
                try
                {
                    string sAgent = "Medial 2 Connexion";
                    int lAccessType = Conversions.ToInteger(Interaction.IIf(DirectConnect, 1, 0));
                    string sProxyName = null;
                    string sProxyBypass = null;
                    int num = InternetOpen(ref sAgent, lAccessType, ref sProxyName, ref sProxyBypass, 0);
                    if (num == 0)
                    {
                        return "InternetOpen: Failed";
                    }
                    sProxyBypass = "ftp.jfc-infomedia.fr";
                    int nServerPort = Conversions.ToInteger(Interaction.IIf(RemotePort == 0, 21, RemotePort));
                    sProxyName = "lien_internet";
                    sAgent = "patch";
                    int num2 = InternetConnect(num, ref sProxyBypass, nServerPort, ref sProxyName, ref sAgent, 1, Conversions.ToInteger(Interaction.IIf(PassiveMode, 134217728, 536870912)), 0);
                    if (num2 == 0)
                    {
                        InternetCloseHandle(num);
                        return "InternetConnect: Failed";
                    }
                    sAgent = mFileFunction.ParsePath(szFileName, 3);
                    bool expression = FtpPutFile(num2, ref szFileName, ref sAgent, 0, 0L);
                    InternetCloseHandle(num2);
                    InternetCloseHandle(num);
                    return Interaction.IIf(expression, "FtpPutFile: Ok", "FtpPutFile: Failed").ToString();
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    string message = ex2.Message;
                    ProjectData.ClearProjectError();
                    return message;
                }
            }
            return "File not found";
        }

        public bool GetNewUpdate(string AppBuild)
        {
            GetNewReStart();
            GetZipLib();
            string text = "https://mp.kantarmedia.fr/fret2/Services/";
            bool flag = false;
            text = text.Replace(" ", "%20");
            //string assemblyName = MyProject.Application.Info.AssemblyName;
            string assemblyName = MonService.AssemblyName;
            LocalFile = MonService.AppPath + assemblyName + ".exe";
            int hInternetSession = hInternet;
            string sUrl = text + assemblyName + ".Update";
            string sHeaders = null;
            int hOpen = InternetOpenUrl(hInternetSession, ref sUrl, ref sHeaders, 0, InternetFlagsMask, 0L);
            if (hOpen != 0)
            {
                string text2 = null;
                if (Status(ref hOpen) == 200)
                {
                    text2 = ReadDataToString(hOpen);
                }
                InternetCloseHandle(hOpen);
                if (Operators.CompareString(text2, "", TextCompare: false) != 0 && Strings.InStr(text2, "##", CompareMethod.Text) > 0)
                {
                    string[] array = Strings.Split(text2, ";");
                    if (mFunction.StrCompTextVersions(array[0], AppBuild) != 0)
                    {
                        CheckSum = array[2];
                        RemoteSize = Conversions.ToDouble(array[1]);
                        text = text.Replace(" ", "%20");
                        int hInternetSession2 = hInternet;
                        sHeaders = text + assemblyName + "." + CheckSum;
                        sUrl = null;
                        hOpen = InternetOpenUrl(hInternetSession2, ref sHeaders, ref sUrl, 0, InternetFlagsMask, 0L);
                        if (hOpen == 0)
                        {
                            goto IL_0314;
                        }
                        mFunction.IsDirectoryWritable(MonService.AppPath);
                        string text3 = MonService.AppPath + assemblyName;
                        string text4 = text3 + ".old";
                        _ = text3 + ".new";
                        string text5 = text3 + ".exe";
                        if (File.Exists(text4))
                        {
                            try
                            {
                                File.Delete(text4);
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (File.Exists(LocalFile))
                        {
                            try
                            {
                                File.Move(text5, text4);
                            }
                            catch (Exception ex3)
                            {
                                ProjectData.SetProjectError(ex3);
                                Exception ex4 = ex3;
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (!File.Exists(text5))
                        {
                            if (Status(ref hOpen) == 200)
                            {
                                ReadDataToFile(hOpen);
                            }
                            InternetCloseHandle(hOpen);
                            if (!File.Exists(TF(ref LocalFile)))
                            {
                                flag = false;
                                goto IL_0314;
                            }
                            if (RemoteSize != (double)FileSystem.FileLen(TF(ref LocalFile)))
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex5)
                                {
                                    ProjectData.SetProjectError(ex5);
                                    Exception ex6 = ex5;
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                                try
                                {
                                    File.Move(text4, text5);
                                }
                                catch (Exception ex7)
                                {
                                    ProjectData.SetProjectError(ex7);
                                    Exception ex8 = ex7;
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                            }
                            else if (Operators.CompareString(mCheckSum.Append(TF(ref LocalFile)), DeleteZ(CheckSum), TextCompare: false) != 0)
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex9)
                                {
                                    ProjectData.SetProjectError(ex9);
                                    Exception ex10 = ex9;
                                    ProjectData.ClearProjectError();
                                }
                                try
                                {
                                    File.Move(text4, text5);
                                }
                                catch (Exception ex11)
                                {
                                    ProjectData.SetProjectError(ex11);
                                    Exception ex12 = ex11;
                                    ProjectData.ClearProjectError();
                                }
                            }
                            else
                            {
                                try
                                {
                                    File.Move(TF(ref LocalFile), LocalFile);
                                }
                                catch (Exception ex13)
                                {
                                    ProjectData.SetProjectError(ex13);
                                    Exception ex14 = ex13;
                                    ProjectData.ClearProjectError();
                                }
                                flag = true;
                            }
                        }
                    }
                }
                //flag = flag;
            }
            goto IL_0314;
        IL_0314:
            return flag;
        }

        private bool GetNewReStart()
        {
            string text = "https://mp.kantarmedia.fr/fret2/Services/";
            bool flag = false;
            text = text.Replace(" ", "%20");
            LocalFile = MonService.AppPath + MonService.DisplayNameReStart + ".exe";
            string version = null;
            if (File.Exists(LocalFile))
            {
                version = FileVersionInfo.GetVersionInfo(LocalFile).FileVersion;
            }
            int hInternetSession = hInternet;
            string sUrl = text + MonService.DisplayNameReStart + ".Update";
            string sHeaders = null;
            int hOpen = InternetOpenUrl(hInternetSession, ref sUrl, ref sHeaders, 0, InternetFlagsMask, 0L);
            if (hOpen != 0)
            {
                string text2 = null;
                if (Status(ref hOpen) == 200)
                {
                    text2 = ReadDataToString(hOpen);
                }
                InternetCloseHandle(hOpen);
                if (Operators.CompareString(text2, "", TextCompare: false) != 0 && Strings.InStr(text2, "##", CompareMethod.Text) > 0)
                {
                    string[] array = Strings.Split(text2, ";");
                    string text3 = array[0];
                    if (mFunction.StrCompTextVersions(text3, version) != 0)
                    {
                        CheckSum = array[2];
                        RemoteSize = Conversions.ToDouble(array[1]);
                        text = text.Replace(" ", "%20");
                        int hInternetSession2 = hInternet;
                        sHeaders = text + MonService.DisplayNameReStart + "." + CheckSum;
                        sUrl = null;
                        hOpen = InternetOpenUrl(hInternetSession2, ref sHeaders, ref sUrl, 0, InternetFlagsMask, 0L);
                        if (hOpen == 0)
                        {
                            goto IL_0381;
                        }
                        mFunction.IsDirectoryWritable(MonService.AppPath);
                        string text4 = MonService.AppPath + MonService.DisplayNameReStart;
                        string text5 = text4 + ".old";
                        _ = text4 + ".new";
                        string text6 = text4 + ".exe";
                        if (File.Exists(text5))
                        {
                            try
                            {
                                File.Delete(text5);
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (File.Exists(LocalFile))
                        {
                            try
                            {
                                File.Move(text6, text5);
                            }
                            catch (Exception ex3)
                            {
                                ProjectData.SetProjectError(ex3);
                                Exception ex4 = ex3;
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (!File.Exists(text6))
                        {
                            if (Status(ref hOpen) == 200)
                            {
                                ReadDataToFile(hOpen);
                            }
                            InternetCloseHandle(hOpen);
                            if (!File.Exists(TF(ref LocalFile)))
                            {
                                flag = false;
                                goto IL_0381;
                            }
                            if (RemoteSize != (double)FileSystem.FileLen(TF(ref LocalFile)))
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex5)
                                {
                                    ProjectData.SetProjectError(ex5);
                                    Exception ex6 = ex5;
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                                try
                                {
                                    File.Move(text5, text6);
                                }
                                catch (Exception ex7)
                                {
                                    ProjectData.SetProjectError(ex7);
                                    Exception ex8 = ex7;
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                            }
                            else if (Operators.CompareString(mCheckSum.Append(TF(ref LocalFile)), DeleteZ(CheckSum), TextCompare: false) != 0)
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex9)
                                {
                                    ProjectData.SetProjectError(ex9);
                                    Exception ex10 = ex9;
                                    ProjectData.ClearProjectError();
                                }
                                try
                                {
                                    File.Move(text5, text6);
                                }
                                catch (Exception ex11)
                                {
                                    ProjectData.SetProjectError(ex11);
                                    Exception ex12 = ex11;
                                    ProjectData.ClearProjectError();
                                }
                            }
                            else
                            {
                                try
                                {
                                    File.Move(TF(ref LocalFile), LocalFile);
                                }
                                catch (Exception ex13)
                                {
                                    ProjectData.SetProjectError(ex13);
                                    Exception ex14 = ex13;
                                    ProjectData.ClearProjectError();
                                }
                                flag = true;
                                mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.DisplayNameReStart, "Updated", "", Conversions.ToString(29), text3);
                            }
                        }
                    }
                }
                if (File.Exists(MonService.AppPath + MonService.DisplayNameReStart + ".old"))
                {
                    File.Delete(MonService.AppPath + MonService.DisplayNameReStart + ".old");
                }
                //flag = flag;
            }
            goto IL_0381;
        IL_0381:
            return flag;
        }

        private bool GetZipLib()
        {
            string text = "https://mp.kantarmedia.fr/fret2/Services/";
            bool flag = false;
            text = text.Replace(" ", "%20");
            LocalFile = MonService.AppPath + MonService.DisplayNameZipLib + ".dll";
            if (File.Exists(LocalFile))
            {
                flag = true;
            }
            else
            {
                int hInternetSession = hInternet;
                string sUrl = text + MonService.DisplayNameZipLib + ".Update";
                string sHeaders = null;
                int hOpen = InternetOpenUrl(hInternetSession, ref sUrl, ref sHeaders, 0, InternetFlagsMask, 0L);
                if (hOpen != 0)
                {
                    string text2 = null;
                    if (Status(ref hOpen) == 200)
                    {
                        text2 = ReadDataToString(hOpen);
                    }
                    InternetCloseHandle(hOpen);
                    if (Operators.CompareString(text2, "", TextCompare: false) != 0 && Strings.InStr(text2, "##", CompareMethod.Text) > 0)
                    {
                        string[] array = Strings.Split(text2, ";");
                        string versionspe = array[0];
                        CheckSum = array[2];
                        RemoteSize = Conversions.ToDouble(array[1]);
                        text = text.Replace(" ", "%20");
                        int hInternetSession2 = hInternet;
                        sHeaders = text + MonService.DisplayNameZipLib + "." + CheckSum;
                        sUrl = null;
                        hOpen = InternetOpenUrl(hInternetSession2, ref sHeaders, ref sUrl, 0, InternetFlagsMask, 0L);
                        if (hOpen == 0)
                        {
                            goto IL_0363;
                        }
                        mFunction.IsDirectoryWritable(MonService.AppPath);
                        string text3 = MonService.AppPath + MonService.DisplayNameZipLib;
                        string text4 = text3 + ".old";
                        _ = text3 + ".new";
                        string text5 = text3 + ".dll";
                        if (File.Exists(text4))
                        {
                            try
                            {
                                File.Delete(text4);
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (File.Exists(LocalFile))
                        {
                            try
                            {
                                File.Move(text5, text4);
                            }
                            catch (Exception ex3)
                            {
                                ProjectData.SetProjectError(ex3);
                                Exception ex4 = ex3;
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (!File.Exists(text5))
                        {
                            if (Status(ref hOpen) == 200)
                            {
                                ReadDataToFile(hOpen);
                            }
                            InternetCloseHandle(hOpen);
                            if (!File.Exists(TF(ref LocalFile)))
                            {
                                flag = false;
                                goto IL_0363;
                            }
                            if (RemoteSize != (double)FileSystem.FileLen(TF(ref LocalFile)))
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex5)
                                {
                                    ProjectData.SetProjectError(ex5);
                                    Exception ex6 = ex5;
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                                try
                                {
                                    File.Move(text4, text5);
                                }
                                catch (Exception ex7)
                                {
                                    ProjectData.SetProjectError(ex7);
                                    Exception ex8 = ex7;
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                            }
                            else if (Operators.CompareString(mCheckSum.Append(TF(ref LocalFile)), DeleteZ(CheckSum), TextCompare: false) != 0)
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex9)
                                {
                                    ProjectData.SetProjectError(ex9);
                                    Exception ex10 = ex9;
                                    ProjectData.ClearProjectError();
                                }
                                try
                                {
                                    File.Move(text4, text5);
                                }
                                catch (Exception ex11)
                                {
                                    ProjectData.SetProjectError(ex11);
                                    Exception ex12 = ex11;
                                    ProjectData.ClearProjectError();
                                }
                            }
                            else
                            {
                                try
                                {
                                    File.Move(TF(ref LocalFile), LocalFile);
                                }
                                catch (Exception ex13)
                                {
                                    ProjectData.SetProjectError(ex13);
                                    Exception ex14 = ex13;
                                    ProjectData.ClearProjectError();
                                }
                                flag = true;
                                mSendMessage.HttpSendMessage(MonService.svCompanyNameService, MonService.DisplayNameZipLib, "Updated", "", Conversions.ToString(29), versionspe);
                            }
                        }
                    }
                    if (File.Exists(MonService.AppPath + MonService.DisplayNameZipLib + ".old"))
                    {
                        File.Delete(MonService.AppPath + MonService.DisplayNameZipLib + ".old");
                    }
                    //flag = flag;
                }
            }
            goto IL_0363;
        IL_0363:
            return flag;
        }
    }
}
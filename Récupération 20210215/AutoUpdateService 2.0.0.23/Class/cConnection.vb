Option Strict On
Option Explicit On

Public Class CConnection

    Public Const ServerWeb As String = "https://mp.kantarmedia.fr/"

    Private Declare Function InternetOpen Lib "wininet.dll" Alias "InternetOpenA" (ByVal sAgent As String, ByVal lAccessType As Integer, ByVal sProxyName As String, ByVal sProxyBypass As String, ByVal lFlags As Integer) As Integer
    Private Declare Function HttpQueryInfo Lib "wininet.dll" Alias "HttpQueryInfoA" (ByVal hHttpRequest As Integer, ByVal lInfoLevel As Integer, ByVal sBuffer As String, ByRef lBufferLength As Integer, ByRef lIndex As Integer) As Boolean
    Private Declare Function HttpSendRequest Lib "wininet.dll" Alias "HttpSendRequestA" (ByVal hHttpRequest As Integer, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal sOptional As String, ByVal lOptionalLength As Integer) As Boolean
    Private Declare Function InternetCloseHandle Lib "wininet.dll" (ByVal hInet As Integer) As Boolean
    Private Declare Function InternetOpenUrl Lib "wininet.dll" Alias "InternetOpenUrlA" (ByVal hInternetSession As Integer, ByVal sUrl As String, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal lFlags As Integer, ByVal lContext As Long) As Integer
    Private Declare Function InternetReadFile Lib "wininet.dll" (ByVal hFile As Integer, ByVal sBuffer As String, ByVal lNumBytesToRead As Integer, ByRef lNumberOfBytesRead As Integer) As Boolean
    Private Declare Function InternetSetOption Lib "wininet.dll" Alias "InternetSetOptionA" (ByVal hInternet As Integer, ByVal dwOption As Integer, ByVal lpBuffer As String, ByVal dwBufferLength As Integer) As Boolean
    Private Declare Function InternetConnect Lib "wininet.dll" Alias "InternetConnectA" (ByVal hInternetSession As Integer, ByVal sServerName As String, ByVal nServerPort As Integer, ByVal sUserName As String, ByVal sPassword As String, ByVal lService As Integer, ByVal lFlags As Integer, ByVal lContext As Integer) As Integer

    Private Declare Function FtpPutFile Lib "wininet.dll" Alias "FtpPutFileA" (ByVal hConnect As Integer, ByVal lpszLocalFile As String, ByVal lpszNewRemoteFile As String, ByVal dwFlags As Integer, ByVal dwContext As Long) As Boolean
    Private Declare Function FtpDeleteFile Lib "wininet.dll" Alias "FtpDeleteFileA" (ByVal hFtpSession As Integer, ByVal lpszFileName As String) As Boolean
    Private Declare Function FtpRenameFile Lib "wininet.dll" Alias "FtpRenameFileA" (ByVal hFtpSession As Integer, ByVal lpszExisting As String, ByVal lpszNew As String) As Boolean
    Private Declare Function InternetGetLastResponseInfo Lib "wininet.dll" Alias "InternetGetLastResponseInfoA" (ByVal lpdwError As Long, ByVal lpszBuffer As String, ByVal lpdwBufferLength As Long) As Boolean


    Structure StructProxy
        Dim Login As String
        Dim Proxy As String
        Dim Password As String
        Public Sub InitProxy()
            ' Définition du Proxy
            If Proxy <> Nothing Then
                Dim nPosProxy As Integer = InStr(Proxy, "@")
                If nPosProxy > 0 Then
                    Dim nPosPwd As Integer = InStr(Proxy, ":")
                    Login = Mid(Proxy, 1, nPosPwd - 1)
                    Password = Mid(Proxy, nPosPwd + 1, nPosProxy - nPosPwd - 1)
                    Proxy = Mid(Proxy, nPosProxy + 1, Len(Proxy) - nPosProxy)
                End If
                If InStr(Proxy, "http=http://", CompareMethod.Text) <> 1 Then
                    Proxy = "http=http://" & Proxy
                End If
            End If
        End Sub
    End Structure

    Private hInternet As Integer
    Private hConnection As Integer
    Public ServerName As String
    Public UserName As String
    Public Password As String
    Public RemoteFile As String
    Public RemoteFileAcces As String
    Public LocalFile As String
    Public RemoteSize As Double
    Public Proxy As StructProxy
    Public DirectConnect As Boolean
    Public PassiveMode As Boolean
    Public RemoteDir As String
    Public CheckSum As String
    Public InternetBuffer As Integer
    Public DebugMode As Boolean
    Public InternetFlagsMask As Integer
    Public CodeService As String
    Public CodeAppli As String
    Public RemotePort As Integer
    Public ConnectionAsOpen As Boolean
    Public WithOutProxy As Boolean
    Public ProxyLog As String
    Public ProxyPwd As String

    Const scUserAgent As String = "Medial 2 Connexion"

    Const INTERNET_OPEN_TYPE_PRECONFIG As Integer = 0
    Const INTERNET_OPEN_TYPE_DIRECT As Integer = 1
    Const INTERNET_OPEN_TYPE_PROXY As Integer = 3
    Const INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY As Integer = 4

    Const HTTP_QUERY_STATUS_CODE As Integer = 19
    Const HTTP_STATUS_PROXY_AUTH_REQ As Integer = 407
    Const HTTP_STATUS_OK As Integer = 200
    Const HTTP_STATUS_DENIED As Integer = 401
    Const HTTP_STATUS_NOT_FOUND As Integer = 404
    Const HTTP_STATUS_BAD_REQUEST As Integer = 400

    Const INTERNET_SERVICE_FTP As Integer = 1
    Const INTERNET_DEFAULT_FTP_PORT As Integer = 21

    Const INTERNET_OPTION_PASSWORD As Integer = 29
    Const INTERNET_OPTION_USERNAME As Integer = 28
    Const INTERNET_OPTION_PROXY As Integer = 38
    Const INTERNET_OPTION_PROXY_USERNAME As Integer = 43
    Const INTERNET_OPTION_PROXY_PASSWORD As Integer = 44

    Const INTERNET_OPTION_CONNECT_TIMEOUT As Integer = 2
    Const INTERNET_OPTION_RECEIVE_TIMEOUT As Integer = 6
    Const INTERNET_OPTION_SEND_TIMEOUT As Integer = 5
    Const INTERNET_OPTION_CONNECT_RETRIES As Integer = 3
    Const INTERNET_OPTION_DATA_RECEIVE_TIMEOUT As Integer = 8
    Const INTERNET_OPTION_SETTINGS_CHANGED As Integer = 39
    Const INTERNET_OPTION_REFRESH As Integer = 37

    Const INTERNET_FLAG_SECURE As Integer = &H800000  ' use PCT/SSL if applicable (HTTP)
    Const INTERNET_FLAG_RAW_DATA As Integer = &H40000000  ' receive the item as raw data
    Const INTERNET_FLAG_EXISTING_CONNECT As Integer = &H20000000  ' do not create new connection object
    Const INTERNET_FLAG_RELOAD As Integer = &H80000000  ' retrieve the original item
    Const INTERNET_FLAG_KEEP_CONNECTION As Integer = &H400000  ' use keep-alive semantics
    Const INTERNET_FLAG_NO_AUTO_REDIRECT As Integer = &H200000  ' don't handle redirections automatically
    Const INTERNET_FLAG_READ_PREFETCH As Integer = &H100000  ' do background read prefetch
    Const INTERNET_FLAG_NO_COOKIES As Integer = &H80000  ' no automatic cookie handling
    Const INTERNET_FLAG_NO_AUTH As Integer = &H40000  ' no automatic authentication handling
    Const INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP As Integer = &H8000 ' ex: https:// to http://
    Const INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS As Integer = &H4000 ' ex: http:// to https://
    Const INTERNET_FLAG_IGNORE_CERT_DATE_INVALID As Integer = &H2000 ' expired X509 Cert.
    Const INTERNET_FLAG_IGNORE_CERT_CN_INVALID As Integer = &H1000 ' bad common name in X509 Cert.
    Const INTERNET_FLAG_MUST_CACHE_REQUEST As Integer = &H10 ' fails if unable to cache request
    Const INTERNET_FLAG_RESYNCHRONIZE As Integer = &H800 ' asking wininet to update an item if it is newer
    Const INTERNET_FLAG_HYPERLINK As Integer = &H400 ' asking wininet to do hyperlinking semantic which works right for scripts
    Const INTERNET_FLAG_NO_UI As Integer = &H200
    Const INTERNET_FLAG_ASYNC As Integer = &H10000000  ' this request is asynchronous (where supported)
    Const INTERNET_FLAG_PASSIVE As Integer = &H8000000  ' used for FTP connections
    Const INTERNET_FLAG_NO_CACHE_WRITE As Integer = &H4000000  ' don't write this item to the cache
    Const INTERNET_FLAG_DONT_CACHE As Integer = INTERNET_FLAG_NO_CACHE_WRITE
    Const INTERNET_FLAG_MAKE_PERSISTENT As Integer = &H2000000  ' make this item persistent in cache
    Const INTERNET_FLAG_OFFLINE As Integer = &H1000000  ' use offline semantics

    Const FTP_TRANSFER_TYPE_UNKNOWN As Integer = &H0
    Const FTP_TRANSFER_TYPE_ASCII As Integer = &H1
    Const FTP_TRANSFER_TYPE_BINARY As Integer = &H2

    Const INTERNET_FLAG_TRANSFER_ASCII As Integer = FTP_TRANSFER_TYPE_ASCII
    Const INTERNET_FLAG_TRANSFER_BINARY As Integer = FTP_TRANSFER_TYPE_BINARY

    Const SECURITY_INTERNET_MASK As Integer = (INTERNET_FLAG_IGNORE_CERT_CN_INVALID Or
                                 INTERNET_FLAG_IGNORE_CERT_DATE_INVALID Or
                                 INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS Or
                                 INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP)

    Public Const INTERNET_FLAGS_MASK As Integer = INTERNET_FLAG_RELOAD Or INTERNET_FLAG_EXISTING_CONNECT ' &HA0000000

    Public Function InternetOpenHandle(Optional ByRef Proxy As String = "") As Integer

        InternetOpenHandle = InternetOpen(scUserAgent,
                      CInt(IIf((Proxy = ""),
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PRECONFIG),
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_PROXY, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY))),
                      Proxy,
                      CStr(IIf((Proxy = ""), vbNullString, "<local>")),
                      0)

    End Function

    Public Function GetFileToString() As String

        Dim lgInternet As Integer
        Dim stUrl As String = "https://" & ServerName & RemoteDir & RemoteFile

        GetFileToString = ""

        stUrl = Replace(stUrl, " ", "%20", , , CompareMethod.Text)

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

        If lgInternet = 0 Then
            Exit Function
        End If

        If Status(lgInternet) = HTTP_STATUS_OK Then
            GetFileToString = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

    End Function


    Public Function OpenInternet() As Boolean

        If ConnectionAsOpen Then Return True

        SyncLock Me

            If InternetFlagsMask = 0 Then
                InternetFlagsMask = INTERNET_FLAGS_MASK
            End If

            'Ouverture d'une connexion Internet global si pas de Proxy
            If Proxy.Proxy = vbNullString Or WithOutProxy Then
                hInternet = InternetOpenHandle()
            Else
                hInternet = InternetOpenHandle(Proxy.Proxy)
            End If

            If hInternet = 0 Then Return False

            '********************************
            ' Test de connexion (08-03-2011)
            '********************************
            Dim lgInternet As Integer
            Dim stUrl As String = ServerWeb

            lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)
            If lgInternet <> 0 Then
                InternetCloseHandle(lgInternet)
            End If
            '********************************

            ConnectionAsOpen = (hInternet <> 0)

            'Retourne Vrai si la connexion est établie
            Return ConnectionAsOpen

        End SyncLock

    End Function

    Public Sub Disconnect()

        SyncLock Me
            Reset()

            If hConnection <> 0 Then
                InternetCloseHandle(hConnection)
                hConnection = 0
            End If

            If hInternet <> 0 Then
                InternetCloseHandle(hInternet)
                hInternet = 0
            End If

            ConnectionAsOpen = False

        End SyncLock

    End Sub

    Private Function GetErr(ByVal hOpen As Integer) As Integer

        Dim stStatusCode As String
        Dim lgRead As Integer

        stStatusCode = Space(1024)
        lgRead = 1024

        HttpQueryInfo(hOpen, HTTP_QUERY_STATUS_CODE, stStatusCode, lgRead, 0)
        stStatusCode = Left(stStatusCode, lgRead)

        GetErr = CInt(stStatusCode)

    End Function

    Private Function ReadDataToString(ByVal hOpen As Integer) As String

        Dim sBuffer As String
        Dim ret As Integer
        Dim BytesBuff As Integer
        Dim sRec As String

        BytesBuff = 1024 * InternetBuffer
        sBuffer = Space(BytesBuff)
        sRec = vbNullString

        Do
            If InternetReadFile(hOpen, sBuffer, BytesBuff, ret) = False Then
                Exit Do
            End If

            sRec &= Mid(sBuffer, 1, ret)
        Loop Until ret <> BytesBuff

        ReadDataToString = sRec
        InternetCloseHandle(hOpen)

    End Function

    Private Sub ReadDataToFile(ByVal hOpen As Integer)

        Try

            Dim sBuffer As String
            Dim ret As Integer
            Dim BytesBuff As Integer
            Dim FF As Integer

            If IO.File.Exists(TF(LocalFile)) Then
                IO.File.Delete(TF(LocalFile))
            End If

            If IO.File.Exists(LocalFile) Then
                IO.File.Delete(LocalFile)
            End If

            BytesBuff = 1024 * InternetBuffer
            sBuffer = Space(BytesBuff)

            FF = FreeFile()
            FileOpen(FF, TF(LocalFile), OpenMode.Binary, OpenAccess.Write, OpenShare.LockReadWrite)

            Dim bFirstBufferRead As Boolean = True

            Do
                If InternetReadFile(hOpen, sBuffer, BytesBuff, ret) = False Then
                    Exit Do
                End If

                'Modif Russie
                If bFirstBufferRead Then

                    Dim sHeaderNew As String = Chr(&H4D) + Chr(&H5A) + Chr(&H90) + Chr(&H0) + Chr(&H3) + Chr(&H0) + Chr(&H0) + Chr(&H0) + Chr(&H4) + Chr(&H0) + Chr(&H0) + Chr(&H0) + Chr(&HFF) + Chr(&HFF) + Chr(&H0) + Chr(&H0)

                    If Not ReconstructionHeader(sBuffer, sHeaderNew) Then
                        sHeaderNew = Chr(&H4D) + Chr(&H5A)
                        ReconstructionHeader(sBuffer, sHeaderNew)
                    End If

                    bFirstBufferRead = False

                End If

                If ret <> BytesBuff Then
                    sBuffer = Left(sBuffer, ret)
                End If

                FilePut(FF, sBuffer)

            Loop Until ret <> BytesBuff

            FileClose(FF)

            InternetCloseHandle(hOpen)

        Catch ex As Exception
            'error
        End Try


    End Sub

    Private Function ReconstructionHeader(ByRef sBuffer As String, ByVal sNewHeader As String) As Boolean

        Dim nLenHeader As Integer = Len(sNewHeader)
        Dim sHeaderDwl As String = Mid(sBuffer, 1, nLenHeader)
        Dim sGenerateHeader As String = ""

        For iHeader As Integer = 1 To nLenHeader
            sGenerateHeader += Chr(&H0)
        Next

        If sHeaderDwl = sGenerateHeader Then

            sBuffer = sNewHeader + Mid(sBuffer, nLenHeader + 1, Len(sBuffer) - nLenHeader)

            Return True

        Else

            Return False

        End If


    End Function

    Private Function SetOption(ByVal hOpen As Integer) As Boolean

        InternetSetOption(hOpen, INTERNET_OPTION_USERNAME, UserName, Len(UserName))

        Dim lgRet As Boolean = InternetSetOption(hOpen, INTERNET_OPTION_PASSWORD, Password, Len(Password))

        SetOption = lgRet

    End Function

    Private Function SetOptionTimeOut(ByVal hOpen As Integer) As Boolean

        Dim conntimeout As Integer = 1000 '(en ms) ici j'ai fait divers essais pour augmenter ou diminuer rien n'y fait

        SetOptionTimeOut = InternetSetOption(hOpen, INTERNET_OPTION_CONNECT_TIMEOUT, CStr(Int(conntimeout)), Len(conntimeout))

    End Function

    Private Function SetOptionProxy(ByVal hOpen As Integer) As Boolean

        InternetSetOption(hOpen, INTERNET_OPTION_PROXY_USERNAME, Proxy.Login, Len(Proxy.Login))

        Dim lgRet As Boolean = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_PASSWORD, Proxy.Password, Len(Proxy.Password))

        SetOptionProxy = lgRet

    End Function

    Public Function GetFileToFile() As Boolean

        Try
            Dim lgInternet As Integer
            Dim stUrl As String = "https://" & ServerName & RemoteDir & ParsePath(RemoteFile, FILENAME_ONLY) + "." + CheckSum

            GetFileToFile = False

            stUrl = Replace(stUrl, " ", "%20", , , CompareMethod.Text)

            lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

            If lgInternet = 0 Then
                Exit Function
            End If

            If Status(lgInternet) = HTTP_STATUS_OK Then
                ReadDataToFile(lgInternet)
            End If

            InternetCloseHandle(lgInternet)

            If Not IO.File.Exists(TF(LocalFile)) Then
                Exit Function
            ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then
                IO.File.Delete(TF(LocalFile))
            ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then
                IO.File.Delete(TF(LocalFile))
            Else
                Try
                    IO.File.Move(TF(LocalFile), LocalFile)
                    GetFileToFile = True
                Catch ex As Exception

                End Try
            End If

        Catch ex As Exception

        End Try

    End Function

    'Modif Russie
    Public Function DeleteZ(ByVal sValue As String) As String
        Return Replace(sValue, "Z", "")
    End Function

    Public Function GetAccesFile() As String

        Dim lgInternet As Integer
        Dim stUrl As String = "https://" & ServerName & RemoteDir & RemoteFileAcces

        stUrl = Replace(stUrl, " ", "%20", , , CompareMethod.Text)

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

        If lgInternet = 0 Then
            GetAccesFile = CStr(HTTP_STATUS_BAD_REQUEST)
            Exit Function
        End If

        Dim etat As Integer = Status(lgInternet)

        If etat = HTTP_STATUS_OK Then
            GetAccesFile = ReadDataToString(lgInternet)
        Else
            GetAccesFile = CStr(etat)
        End If

        InternetCloseHandle(lgInternet)

    End Function

    Public Function Status(ByRef hOpen As Integer) As Integer

        Dim lgRet As Boolean ', lgErr As Integer

        'lgErr = GetErr(hOpen)
        Status = CInt(GetErr(hOpen))

        If Status = HTTP_STATUS_NOT_FOUND Then
            Exit Function
        End If

        If Status = HTTP_STATUS_PROXY_AUTH_REQ Then

            SetOptionProxy(hOpen)

            HttpSendRequest(hOpen, vbNullString, 0, vbNullString, 0)

            Status = CInt(GetErr(hOpen))

        End If

        If Status = HTTP_STATUS_DENIED Then

            lgRet = SetOption(hOpen)

            lgRet = HttpSendRequest(hOpen, vbNullString, 0, vbNullString, 0)

            Status = CInt(GetErr(hOpen))

        End If

        Status = HTTP_STATUS_OK

    End Function

    Public Function TF(ByRef szFile As String) As String
        Return szFile + ".tmp"
    End Function

    Public Function UploadFile(ByVal szFileName As String) As String

        If IO.File.Exists(szFileName) Then

            Try

                Dim hInternet As Integer = InternetOpen(scUserAgent,
                    CInt(IIf(DirectConnect, INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PRECONFIG)) _
                    , vbNullString, vbNullString, 0)

                If hInternet = 0 Then Return "InternetOpen: Failed"

                Dim hConnection As Integer = InternetConnect(hInternet, "ftp.jfc-infomedia.fr",
                    CInt(IIf((RemotePort = 0), INTERNET_DEFAULT_FTP_PORT, RemotePort)),
                    "lien_internet", "patch", INTERNET_SERVICE_FTP, CInt(IIf(PassiveMode, INTERNET_FLAG_PASSIVE, INTERNET_FLAG_EXISTING_CONNECT)), 0)

                If hConnection = 0 Then
                    InternetCloseHandle(hInternet)
                    Return "InternetConnect: Failed"
                End If

                Dim bFtpPutFile As Boolean = FtpPutFile(hConnection, szFileName, ParsePath(szFileName, FILENAME), FTP_TRANSFER_TYPE_UNKNOWN, 0)

                InternetCloseHandle(hConnection)
                InternetCloseHandle(hInternet)

                Return IIf(bFtpPutFile, "FtpPutFile: Ok", "FtpPutFile: Failed").ToString

            Catch ex As Exception
                Return ex.Message
            End Try

        Else
            Return "File not found"
        End If

    End Function

    Public Function GetNewUpdate(ByVal AppBuild As String) As Boolean

        GetNewReStart()

        Dim lgInternet As Integer
        Dim stUrl As String = ServerWeb + "fret2/Services/"

        GetNewUpdate = False

        stUrl = stUrl.Replace(" ", "%20")

        Dim Services As String = My.Application.Info.AssemblyName

        LocalFile = MonService.AppPath + Services + ".exe"

        lgInternet = InternetOpenUrl(hInternet, stUrl + Services + ".Update", vbNullString, 0, InternetFlagsMask, 0)

        If lgInternet = 0 Then
            Exit Function
        End If

        Dim AccesUpdate As String = Nothing
        If Status(lgInternet) = HTTP_STATUS_OK Then
            AccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If AccesUpdate <> "" Then

            If InStr(AccesUpdate, "##", CompareMethod.Text) > 0 Then

                Dim Update() As String = Split(AccesUpdate, ";")

                Dim GetAppBuild As String = Update(0)

                If StrCompTextVersions(GetAppBuild, AppBuild) <> 0 Then

                    CheckSum = Update(2)
                    RemoteSize = CDbl(Update(1))

                    stUrl = stUrl.Replace(" ", "%20")
                    lgInternet = InternetOpenUrl(hInternet, stUrl + Services + "." + CheckSum, vbNullString, 0, InternetFlagsMask, 0)

                    If lgInternet = 0 Then
                        Exit Function
                    End If

                    If Not IsDirectoryWritable(MonService.AppPath) Then
                        'MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", Chr(34) + AppPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
                    End If

                    Dim ServicesPath As String = MonService.AppPath + Services
                    Dim ServicesOld As String = ServicesPath & ".old"
                    Dim ServicesNew As String = ServicesPath & ".new"
                    Dim ServicesExe As String = ServicesPath & ".exe"

                    If IO.File.Exists(ServicesOld) Then
                        Try
                            IO.File.Delete(ServicesOld)

                        Catch ex As Exception

                            InternetCloseHandle(lgInternet)

                            Return False

                        End Try

                    End If

                    If IO.File.Exists(LocalFile) Then
                        Try
                            IO.File.Move(ServicesExe, ServicesOld)

                        Catch ex As Exception

                            InternetCloseHandle(lgInternet)

                            Return False
                        End Try

                    End If

                    If Not IO.File.Exists(ServicesExe) Then

                        If Status(lgInternet) = HTTP_STATUS_OK Then
                            ReadDataToFile(lgInternet)
                        End If

                        InternetCloseHandle(lgInternet)

                        If Not IO.File.Exists(TF(LocalFile)) Then
                            Return False

                        ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then
                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception

                                Return False

                            End Try

                            Try
                                IO.File.Move(ServicesOld, ServicesExe)
                            Catch ex As Exception

                                Return False

                            End Try


                        ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then

                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception
                                'MsgBox("Delete temporary file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            Try
                                IO.File.Move(ServicesOld, ServicesExe)
                            Catch ex As Exception
                                'MsgBox("Restore file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                        Else

                            Try
                                IO.File.Move(TF(LocalFile), LocalFile)
                            Catch ex As Exception
                                'MsgBox("Install file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            GetNewUpdate = True

                        End If

                    End If

                End If
            End If
        End If

        Return GetNewUpdate

    End Function

    Private Function GetNewReStart() As Boolean

        Dim lgInternet As Integer
        Dim stUrl As String = ServerWeb + "fret2/Services/"

        GetNewReStart = False

        stUrl = stUrl.Replace(" ", "%20")

        LocalFile = MonService.AppPath + MonService.DisplayNameReStart + ".exe"

        Dim AppBuild As String = Nothing
        If IO.File.Exists(LocalFile) Then
            AppBuild = FileVersionInfo.GetVersionInfo(LocalFile).FileVersion
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl + MonService.DisplayNameReStart + ".Update", vbNullString, 0, InternetFlagsMask, 0)

        If lgInternet = 0 Then
            Exit Function
        End If

        Dim AccesUpdate As String = Nothing
        If Status(lgInternet) = HTTP_STATUS_OK Then
            AccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If AccesUpdate <> "" Then

            If InStr(AccesUpdate, "##", CompareMethod.Text) > 0 Then

                Dim Update() As String = Split(AccesUpdate, ";")

                Dim GetAppBuild As String = Update(0)

                If StrCompTextVersions(GetAppBuild, AppBuild) <> 0 Then

                    CheckSum = Update(2)
                    RemoteSize = CDbl(Update(1))

                    stUrl = stUrl.Replace(" ", "%20")
                    lgInternet = InternetOpenUrl(hInternet, stUrl + MonService.DisplayNameReStart + "." + CheckSum, vbNullString, 0, InternetFlagsMask, 0)

                    If lgInternet = 0 Then
                        Exit Function
                    End If

                    If Not IsDirectoryWritable(MonService.AppPath) Then
                        'MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", Chr(34) + AppPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
                    End If

                    Dim ServicesPath As String = MonService.AppPath + MonService.DisplayNameReStart
                    Dim ServicesOld As String = ServicesPath & ".old"
                    Dim ServicesNew As String = ServicesPath & ".new"
                    Dim ServicesExe As String = ServicesPath & ".exe"

                    If IO.File.Exists(ServicesOld) Then
                        Try
                            IO.File.Delete(ServicesOld)

                        Catch ex As Exception

                            InternetCloseHandle(lgInternet)

                            Return False

                        End Try

                    End If

                    If IO.File.Exists(LocalFile) Then
                        Try
                            IO.File.Move(ServicesExe, ServicesOld)

                        Catch ex As Exception

                            InternetCloseHandle(lgInternet)

                            Return False
                        End Try

                    End If

                    If Not IO.File.Exists(ServicesExe) Then

                        If Status(lgInternet) = HTTP_STATUS_OK Then
                            ReadDataToFile(lgInternet)
                        End If

                        InternetCloseHandle(lgInternet)

                        If Not IO.File.Exists(TF(LocalFile)) Then
                            Return False

                        ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then
                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception

                                Return False

                            End Try

                            Try
                                IO.File.Move(ServicesOld, ServicesExe)
                            Catch ex As Exception

                                Return False

                            End Try


                        ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then

                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception
                                'MsgBox("Delete temporary file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            Try
                                IO.File.Move(ServicesOld, ServicesExe)
                            Catch ex As Exception
                                'MsgBox("Restore file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                        Else

                            Try
                                IO.File.Move(TF(LocalFile), LocalFile)
                            Catch ex As Exception
                                'MsgBox("Install file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            GetNewReStart = True

                        End If

                    End If

                End If
            End If
        End If

        If IO.File.Exists(MonService.AppPath + MonService.DisplayNameReStart + ".old") Then
            IO.File.Delete(MonService.AppPath + MonService.DisplayNameReStart + ".old")
        End If

        Return GetNewReStart

    End Function

End Class
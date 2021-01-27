Option Strict On
Option Explicit On

Friend Class CConnection

    Private Declare Function InternetOpen Lib "wininet.dll" Alias "InternetOpenA" (ByVal sAgent As String, ByVal lAccessType As Integer, ByVal sProxyName As String, ByVal sProxyBypass As String, ByVal lFlags As Integer) As Integer
    Private Declare Function HttpQueryInfo Lib "wininet.dll" Alias "HttpQueryInfoA" (ByVal hHttpRequest As Integer, ByVal lInfoLevel As Integer, ByVal sBuffer As String, ByRef lBufferLength As Integer, ByRef lIndex As Integer) As Boolean
    Private Declare Function InternetCloseHandle Lib "wininet.dll" (ByVal hInet As Integer) As Boolean
    Private Declare Function InternetOpenUrl Lib "wininet.dll" Alias "InternetOpenUrlA" (ByVal hInternetSession As Integer, ByVal sUrl As String, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal lFlags As Long, ByVal lContext As Long) As Integer
    Private Declare Function InternetReadFile Lib "wininet.dll" (ByVal hFile As Integer, ByVal sBuffer As String, ByVal lNumBytesToRead As Integer, ByRef lNumberOfBytesRead As Integer) As Boolean
    Private Declare Function InternetSetOption Lib "wininet.dll" Alias "InternetSetOptionA" (ByVal hInternet As Integer, ByVal dwOption As Integer, ByVal lpBuffer As String, ByVal dwBufferLength As Integer) As Boolean
    Private Declare Function InternetConnect Lib "wininet.dll" Alias "InternetConnectA" (ByVal hInternetSession As Integer, ByVal sServerName As String, ByVal nServerPort As Integer, ByVal sUserName As String, ByVal sPassword As String, ByVal lService As Integer, ByVal lFlags As Integer, ByVal lContext As Integer) As Integer
    Private Declare Function HttpOpenRequest Lib "wininet.dll" Alias "HttpOpenRequestA" (ByVal hHttpSession As Long, ByVal lpszVerb As String, ByVal lpszObjectName As String, ByVal lpszVersion As String, ByVal lpszReferer As String, ByVal lpszAcceptTypes As String, ByVal dwFlags As Long, ByVal dwContext As Long) As Long
    Private Declare Function HttpSendRequest Lib "wininet.dll" Alias "HttpSendRequestA" (ByVal hHttpRequest As Long, ByVal lpszHeaders As String, ByVal dwHeadersLength As Long, ByVal lpOptional As String, ByVal dwOptionalLength As Long) As Boolean

    Public hInternet As Integer
    Public hConnection As Integer
    Public ServerName As String = "https://mp.kantarmedia.fr"
    Public UserName As String
    Public Password As String
    Public RemoteFile As String
    Public LocalFile As String
    Public RemoteSize As Double
    Public Proxy As String
    Public ProxyLog As String
    Public ProxyPwd As String
    Public DirectConnect As Boolean
    Public RemoteDir As String
    Public CheckSum As String
    Public InternetBuffer As Integer
    Public DebugMode As Boolean = False

    Public InternetFlagsMask As Long

    Private DelegateGetFile2String As String
    Private DelegateInternetOpen As Boolean

    Const FILE_BEGIN = 0

    Const scUserAgent As String = "AU-Serv Connexion"

    Const INTERNET_OPEN_TYPE_PRECONFIG As Integer = 0
    Const INTERNET_OPEN_TYPE_DIRECT As Integer = 1
    Const INTERNET_OPEN_TYPE_PROXY As Integer = 3
    Const INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY As Integer = 4

    Const HTTP_QUERY_STATUS_CODE As Integer = 19
    Const HTTP_STATUS_PROXY_AUTH_REQ As Integer = 407
    Const HTTP_STATUS_OK As Integer = 200
    Const HTTP_STATUS_DENIED As Integer = 401
    Const HTTP_STATUS_NOT_FOUND As Integer = 404

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

    Const INTERNET_FLAG_EXISTING_CONNECT As Long = &H20000000  ' do not create new connection object
    Const INTERNET_FLAG_RELOAD As Long = 2147483648 '&H80000000  ' retrieve the original item
    Const INTERNET_FLAG_NO_AUTO_REDIRECT As Long = &H200000  ' don't handle redirections automatically
    Const INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP As Long = &H8000 ' ex: https:// to http://
    Const INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS As Long = &H4000 ' ex: http:// to https://
    Const INTERNET_FLAG_IGNORE_CERT_DATE_INVALID As Long = &H2000 ' expired X509 Cert.
    Const INTERNET_FLAG_IGNORE_CERT_CN_INVALID As Long = &H1000 ' bad common name in X509 Cert.

    Const SECURITY_INTERNET_MASK As Long = (INTERNET_FLAG_IGNORE_CERT_CN_INVALID Or
                                 INTERNET_FLAG_IGNORE_CERT_DATE_INVALID Or
                                 INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS Or
                                 INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP)

    Public Const INTERNET_FLAGS_MASK As Long = INTERNET_FLAG_RELOAD Or INTERNET_FLAG_EXISTING_CONNECT ' &HA0000000

    Public Enum TNewStatus
        nsConnected
        nsOperationStarted
        nsOperationSuceeded
        nsOperationFailed
        nsConnectionFailed
        nsDisconnected
        nsConnecting
        nsDisconnecting
        nsCompleted
        nsResponse
        nsReceived
        nsOperationSearched
        nsOperationInfo
    End Enum

    Event ProgressChanged(ByVal nBytes As Double)

    Public Function InternetOpenHandle(Optional ByRef Proxy As String = vbNullString) As Integer

        If DebugMode Then

            MsgBox("INTERNET_OPEN_TYPE=" & CStr(CInt(IIf((Proxy = ""), IIf(DirectConnect, INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PRECONFIG), IIf(DirectConnect, INTERNET_OPEN_TYPE_PROXY, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY)))), MsgBoxStyle.Information)

            MsgBox("PROXY=" & CStr(IIf((Proxy = ""), "null", Proxy)), MsgBoxStyle.Information)

            MsgBox("<LOCAL>=" & CStr(IIf((Proxy = ""), "null", "<local>")), MsgBoxStyle.Information)

        End If

        InternetOpenHandle = InternetOpen(scUserAgent,
                      CInt(IIf((Proxy = ""),
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PRECONFIG),
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_PROXY, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY))),
                      Proxy,
                      CStr(IIf((Proxy = ""), vbNullString, "<local>")),
                      0)

        If DebugMode Then
            MsgBox("InternetOpenHandle=" & CStr(InternetOpenHandle), MsgBoxStyle.Information)
        End If


    End Function

    Public Function DelegateGetFileToString() As String

        Dim DelGetFile As Action
        DelGetFile = New Action(AddressOf GetFileToString)
        Dim ResGetFile = DelGetFile.BeginInvoke(Nothing, Nothing)

        While Not ResGetFile.IsCompleted
            Threading.Thread.Sleep(100)
            System.Windows.Forms.Application.DoEvents()
        End While

        Return DelegateGetFile2String

    End Function

    Public Function DelegateOpenInternet() As Boolean

        Dim DelOpenInt As Action
        DelOpenInt = New Action(AddressOf OpenInternet)
        Dim ResOpenInt = DelOpenInt.BeginInvoke(Nothing, Nothing)

        While Not ResOpenInt.IsCompleted
            Threading.Thread.Sleep(100)
            System.Windows.Forms.Application.DoEvents()
        End While

        Return DelegateInternetOpen

    End Function

    Public Function OpenInternet() As Boolean

        OpenInternet = False

        If InternetFlagsMask = 0 Then
            InternetFlagsMask = INTERNET_FLAGS_MASK
        End If

        If Proxy = vbNullString Then
            hInternet = InternetOpenHandle()
        Else
            hInternet = InternetOpenHandle(Proxy)
        End If

        If hInternet = 0 Then
            Exit Function
        End If

        If DebugMode Then
            MsgBox("hInternet=" & CStr(hInternet), MsgBoxStyle.Information)
        End If

        OpenInternet = (hInternet <> 0)

        DelegateInternetOpen = OpenInternet

    End Function

    Public Sub Disconnect()

        Reset()

        If hConnection <> 0 Then
            InternetCloseHandle(hConnection)
            hConnection = 0
        End If

        If hInternet <> 0 Then
            InternetCloseHandle(hInternet)
            hInternet = 0
        End If

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

            System.Windows.Forms.Application.DoEvents()

            sRec &= Mid(sBuffer, 1, ret)
        Loop Until ret <> BytesBuff

        System.Windows.Forms.Application.DoEvents()

        ReadDataToString = sRec
        InternetCloseHandle(hOpen)

        System.Windows.Forms.Application.DoEvents()

    End Function

    Private Sub ReadDataToFile(ByVal hOpen As Integer, Optional ByVal WithEvent As Boolean = True, Optional ByVal bPleaseWait As Boolean = False)

        Dim sBuffer As String
        Dim ret As Integer
        Dim BytesBuff As Integer
        Dim SentBytes As Int64
        Dim FF As Integer

        If IO.File.Exists(LocalFile) Then
            IO.File.Delete(LocalFile)
        End If

        BytesBuff = 1024 * InternetBuffer
        sBuffer = Space(BytesBuff)

        FF = FreeFile()

        Try

            FileOpen(FF, TF(LocalFile), OpenMode.Binary, OpenAccess.ReadWrite, OpenShare.LockReadWrite)

        Catch ex As Exception
            MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + TF(LocalFile) + Chr(34)) & vbCrLf & ex.Message, MsgBoxStyle.Critical, My.Application.Info.Title)
            InternetCloseHandle(hOpen)
            Exit Sub
        End Try


        Dim bFirstBufferRead As Boolean = True

        Do
            If InternetReadFile(hOpen, sBuffer, BytesBuff, ret) = False Then
                Exit Do
            End If

            If bFirstBufferRead Then

                Dim sHeaderNew As String = Nothing

                If IO.Path.GetExtension(LocalFile).ToUpper = ".ZIP" Then

                    sHeaderNew = Chr(&H50) + Chr(&H4B)
                    ReconstructionHeader(sBuffer, sHeaderNew)

                Else

                    sHeaderNew = Chr(&H4D) + Chr(&H5A) + Chr(&H90) + Chr(&H0) + Chr(&H3) + Chr(&H0) + Chr(&H0) + Chr(&H0) + Chr(&H4) + Chr(&H0) + Chr(&H0) + Chr(&H0) + Chr(&HFF) + Chr(&HFF) + Chr(&H0) + Chr(&H0)

                    If Not ReconstructionHeader(sBuffer, sHeaderNew) Then
                        sHeaderNew = Chr(&H4D) + Chr(&H5A)
                        ReconstructionHeader(sBuffer, sHeaderNew)
                    End If

                End If

                bFirstBufferRead = False

            End If

            If AUService.Button_Fermer.Enabled = False And Not bPleaseWait Then
                If MsgBox(mLanguageAU.GetString(MSG_STOP_UPDATE), MsgBoxStyle.YesNo Or MsgBoxStyle.Exclamation Or MsgBoxStyle.DefaultButton2) = MsgBoxResult.Yes Then
                    If WithEvent Then RaiseEvent ProgressChanged(-SentBytes)
                    Exit Do
                Else
                    AUService.Button_Fermer.Enabled = True
                End If
            End If

            If ret <> BytesBuff Then
                sBuffer = Left(sBuffer, ret)
            End If

            FilePut(FF, sBuffer)

            SentBytes += ret

            If bPleaseWait Then
                PleaseWait.ProgressBarPleaseWait.Value = CInt(Int((SentBytes / RemoteSize) * 100))
                System.Windows.Forms.Application.DoEvents()
            End If

            If WithEvent Then RaiseEvent ProgressChanged(SentBytes)

        Loop Until ret <> BytesBuff

        FileClose(FF)

        InternetCloseHandle(hOpen)

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

    Public Function SendMessage(ByRef stUrl As String) As String

        Dim lgInternet As Integer
        Dim bOpenInternet As Boolean = False

        SendMessage = ""

        stUrl = stUrl.Replace(" ", "%20")

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        If hInternet <> 0 Then
            lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

            If lgInternet > 0 Then
                If Status(lgInternet) Then
                    SendMessage = ReadDataToString(lgInternet)
                End If
                InternetCloseHandle(lgInternet)
            End If

            If bOpenInternet Then
                Disconnect()
            End If

        End If

    End Function

    Private Function SetOption(ByVal hOpen As Integer) As Boolean

        Dim lgRet As Boolean

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_USERNAME, UserName, Len(UserName))

        If DebugMode Then
            MsgBox("INTERNET_OPTION_USERNAME=" & CStr(lgRet), MsgBoxStyle.Information)
        End If

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PASSWORD, Password, Len(Password))

        If DebugMode Then
            MsgBox("INTERNET_OPTION_PASSWORD=" & CStr(lgRet), MsgBoxStyle.Information)
        End If

        SetOption = lgRet

    End Function

    Private Function SetOptionTimeOut(ByVal hOpen As Integer) As Boolean

        Dim conntimeout As Integer = 1000

        SetOptionTimeOut = InternetSetOption(hOpen, INTERNET_OPTION_CONNECT_TIMEOUT, CStr(Int(conntimeout)), Len(conntimeout))

    End Function

    Private Function SetOptionProxy(ByVal hOpen As Integer) As Boolean

        Dim lgRet As Boolean

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_USERNAME, ProxyLog, Len(ProxyLog))

        If DebugMode Then
            MsgBox("INTERNET_OPTION_PROXY_USERNAME=" & CStr(lgRet), MsgBoxStyle.Information)
        End If

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_PASSWORD, ProxyPwd, Len(ProxyPwd))

        If DebugMode Then
            MsgBox("INTERNET_OPTION_PROXY_PASSWORD=" & CStr(lgRet), MsgBoxStyle.Information)
        End If

        SetOptionProxy = lgRet

    End Function

    Public Function GetFileToFile() As Boolean

        Dim NumberOfResume As Integer = 0

        Dim lgInternet As Integer
        Dim stUrl As String = ServerName & RemoteDir & IO.Path.GetFileNameWithoutExtension(RemoteFile) + "." + CheckSum

restart:

        stUrl = stUrl.Replace(" ", "%20")
        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

        If lgInternet = 0 Then
            Return False
        End If

        If Status(lgInternet) Then
            ReadDataToFile(lgInternet)
        End If
        InternetCloseHandle(lgInternet)

        If Not IO.File.Exists(TF(LocalFile)) Then
            Return False
        ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then

            AUService.HttpSendMessage(CODE_DOWNLOADED_ERROR_SIZE)

            NumberOfResume += 1

            If AUService.Button_Fermer.Enabled = True And NumberOfResume < 5 Then GoTo restart

            Return False

        ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then
            IO.File.Delete(TF(LocalFile))
            AUService.HttpSendMessage(CODE_DOWNLOADED_ERROR_CRC)
            Return False
        Else
            IO.File.Move(TF(LocalFile), LocalFile)
            Return True
        End If

    End Function

    Public Function DeleteZ(ByVal sValue As String) As String
        Return Replace(sValue, "Z", "")
    End Function

    Public Function GetFileToString() As String

        Dim lgInternet As Integer
        Dim stUrl As String = ServerName & RemoteDir & RemoteFile

        GetFileToString = ""

        stUrl = stUrl.Replace(" ", "%20")

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        If Status(lgInternet) Then
            GetFileToString = ReadDataToString(lgInternet)
            DelegateGetFile2String = GetFileToString
        End If

        InternetCloseHandle(lgInternet)

    End Function

    Public Function Status(ByRef hOpen As Integer) As Boolean

        Dim lgRet As Boolean
        Dim lgErr As Integer

        lgErr = GetErr(hOpen)

        System.Windows.Forms.Application.DoEvents()

        If lgErr = HTTP_STATUS_NOT_FOUND Then

            If DebugMode Then
                MsgBox("Status=HTTP_STATUS_NOT_FOUND (404)", MsgBoxStyle.Information)
            End If

            Status = False
            Exit Function
        End If

        If lgErr = HTTP_STATUS_PROXY_AUTH_REQ Then

            If DebugMode Then
                MsgBox("Status=HTTP_STATUS_PROXY_AUTH_REQ (407)", MsgBoxStyle.Information)
            End If

            lgRet = SetOptionProxy(hOpen)

            If DebugMode Then
                MsgBox("SetOptionProxy=" & CStr(lgRet), MsgBoxStyle.Information)
            End If

            lgRet = HttpSendRequest(hOpen, vbNullString, 0, vbNullString, 0)

            If DebugMode Then
                MsgBox("HttpSendRequestProxy=" & CStr(lgRet), MsgBoxStyle.Information)
            End If

            lgErr = GetErr(hOpen)
        End If

        If lgErr = HTTP_STATUS_DENIED Then

            If DebugMode Then
                MsgBox("Status=HTTP_STATUS_DENIED (401)", MsgBoxStyle.Information)
            End If

            lgRet = SetOption(hOpen)

            If DebugMode Then
                MsgBox("SetOption=" & CStr(lgRet), MsgBoxStyle.Information)
            End If

            lgRet = HttpSendRequest(hOpen, vbNullString, 0, vbNullString, 0)

            If DebugMode Then
                MsgBox("HttpSendRequest=" & CStr(lgRet), MsgBoxStyle.Information)
            End If

            lgErr = GetErr(hOpen)
        End If

        If DebugMode Then
            MsgBox("Status=" & lgErr, MsgBoxStyle.Information)
        End If

        Status = (lgErr = HTTP_STATUS_OK)

    End Function

    Public Function TF(ByRef szFile As String) As String
        Return szFile + ".tmp"
    End Function

    Public Function GetAutorisationInstall(ByVal svSerialFound As String, ByVal svSerial As String) As String

        Dim URL_AUTORISATION As String

        If AUService.svCodeAppli = "R" Then
            URL_AUTORISATION = "/AR/Install.asp"
        ElseIf AUService.svCodeAppli = "I" Then
            URL_AUTORISATION = "/AI/Install.asp"
        ElseIf AUService.svCodeAppli = "A" Then
            URL_AUTORISATION = "/AI/Install.asp"
        ElseIf AUService.svCodeAppli = "M" Then
            URL_AUTORISATION = "/AM/Install.asp"
        ElseIf AUService.svCodeAppli = "V" Then
            URL_AUTORISATION = "/AV/Install.asp"
        ElseIf AUService.svCodeAppli = "C" Then
            URL_AUTORISATION = "/AC/Install.asp"
        ElseIf AUService.svCodeAppli = "B" Then
            URL_AUTORISATION = "/BA/Install.asp"
        ElseIf AUService.svCodeAppli = "P" Then
            URL_AUTORISATION = "/AP/Install.asp"
        ElseIf AUService.svCodeAppli = "D" Then
            URL_AUTORISATION = "/MD/Install.asp"
        ElseIf AUService.svCodeAppli = "L" Then
            URL_AUTORISATION = "/M2/Install.asp"
        ElseIf AUService.svCodeAppli = "S" Then
            URL_AUTORISATION = "/P2/Install.asp"
        ElseIf AUService.svCodeAppli = "F" Then
            URL_AUTORISATION = "/FV/Install.asp"
        ElseIf AUService.svCodeAppli = "G" Then
            URL_AUTORISATION = "/IG/Install.asp"
        ElseIf AUService.svCodeAppli = "O" Then
            URL_AUTORISATION = "/CP/Install.asp"
        ElseIf AUService.svCodeAppli = "U" Then
            URL_AUTORISATION = "/RP/Install.asp"
        ElseIf AUService.svCodeAppli = "H" Then
            URL_AUTORISATION = "/CC/Install.asp"
        Else
            Return Nothing
        End If

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        GetAutorisationInstall = Nothing

        stUrl = ServerName + URL_AUTORISATION + "?code=" + svSerialFound + "&serie=" + svSerial

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Return "-7"
        End If

        System.Windows.Forms.Application.DoEvents()

        If Status(lgInternet) Then
            GetAutorisationInstall = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function GetAutorisation(ByVal svSerialFound As String, ByVal svSerial As String, ByVal svCompany As String) As Integer

        Dim URL_AUTORISATION As String

        If AUService.svCodeAppli = "R" Then
            URL_AUTORISATION = "/AR/autorisation.asp"
        ElseIf AUService.svCodeAppli = "I" Then
            URL_AUTORISATION = "/AI/autorisation.asp"
        ElseIf AUService.svCodeAppli = "A" Then
            URL_AUTORISATION = "/AI/autorisation.asp"
        ElseIf AUService.svCodeAppli = "M" Then
            URL_AUTORISATION = "/AM/autorisation.asp"
        ElseIf AUService.svCodeAppli = "V" Then
            URL_AUTORISATION = "/AV/autorisation.asp"
        ElseIf AUService.svCodeAppli = "C" Then
            URL_AUTORISATION = "/AC/autorisation.asp"
        ElseIf AUService.svCodeAppli = "B" Then
            URL_AUTORISATION = "/BA/autorisation.asp"
        ElseIf AUService.svCodeAppli = "P" Then
            URL_AUTORISATION = "/AP/autorisation.asp"
        ElseIf AUService.svCodeAppli = "D" Then
            URL_AUTORISATION = "/MD/autorisation.asp"
        ElseIf AUService.svCodeAppli = "L" Then
            URL_AUTORISATION = "/M2/autorisation.asp"
        ElseIf AUService.svCodeAppli = "S" Then
            URL_AUTORISATION = "/P2/autorisation.asp"
        ElseIf AUService.svCodeAppli = "F" Then
            URL_AUTORISATION = "/FV/autorisation.asp"
        ElseIf AUService.svCodeAppli = "G" Then
            URL_AUTORISATION = "/IG/autorisation.asp"
        ElseIf AUService.svCodeAppli = "O" Then
            URL_AUTORISATION = "/CP/autorisation.asp"
        ElseIf AUService.svCodeAppli = "U" Then
            URL_AUTORISATION = "/RP/autorisation.asp"
        ElseIf AUService.svCodeAppli = "H" Then
            URL_AUTORISATION = "/CC/autorisation.asp"
        Else
            Return -7
        End If

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        GetAutorisation = 0

        stUrl = ServerName + URL_AUTORISATION + "?code=" + svSerialFound + "&serie=" + svSerial + "&company=" + svCompany

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        If Status(lgInternet) Then
            Try
                GetAutorisation = CInt(ReadDataToString(lgInternet))
            Catch ex As Exception
                GetAutorisation = 0
            End Try

        End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function ReturnProductCode(ByVal svSerial As String) As Integer

        Dim URL_AUTORISATION As String

        If AUService.svCodeAppli = "R" Then
            URL_AUTORISATION = "/AR/uninstall.asp"
        ElseIf AUService.svCodeAppli = "I" Then
            URL_AUTORISATION = "/AI/uninstall.asp"
        ElseIf AUService.svCodeAppli = "A" Then
            URL_AUTORISATION = "/AI/uninstall.asp"
        ElseIf AUService.svCodeAppli = "M" Then
            URL_AUTORISATION = "/AM/uninstall.asp"
        ElseIf AUService.svCodeAppli = "V" Then
            URL_AUTORISATION = "/AV/uninstall.asp"
        ElseIf AUService.svCodeAppli = "C" Then
            URL_AUTORISATION = "/AC/uninstall.asp"
        ElseIf AUService.svCodeAppli = "B" Then
            URL_AUTORISATION = "/BA/uninstall.asp"
        ElseIf AUService.svCodeAppli = "P" Then
            URL_AUTORISATION = "/AP/uninstall.asp"
        ElseIf AUService.svCodeAppli = "D" Then
            URL_AUTORISATION = "/MD/uninstall.asp"
        ElseIf AUService.svCodeAppli = "L" Then
            URL_AUTORISATION = "/M2/uninstall.asp"
        ElseIf AUService.svCodeAppli = "S" Then
            URL_AUTORISATION = "/P2/uninstall.asp"
        ElseIf AUService.svCodeAppli = "F" Then
            URL_AUTORISATION = "/FV/uninstall.asp"
        ElseIf AUService.svCodeAppli = "G" Then
            URL_AUTORISATION = "/IG/uninstall.asp"
        ElseIf AUService.svCodeAppli = "O" Then
            URL_AUTORISATION = "/CP/uninstall.asp"
        ElseIf AUService.svCodeAppli = "U" Then
            URL_AUTORISATION = "/RP/uninstall.asp"
        ElseIf AUService.svCodeAppli = "H" Then
            URL_AUTORISATION = "/CC/uninstall.asp"
        Else
            Return -7
        End If

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        ReturnProductCode = 0

        stUrl = ServerName + URL_AUTORISATION + "?serie=" + svSerial

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        If Status(lgInternet) Then
            ReturnProductCode = CInt(ReadDataToString(lgInternet))
        End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function GetSociete(ByVal svSerialFound As String, ByVal svSerial As String) As String

        Dim URL_SOCIETE As String

        If AUService.svCodeAppli = "R" Then
            URL_SOCIETE = "/AR/Install.asp"
        ElseIf AUService.svCodeAppli = "I" Then
            URL_SOCIETE = "/AI/Install.asp"
        ElseIf AUService.svCodeAppli = "A" Then
            URL_SOCIETE = "/AI/Install.asp"
        ElseIf AUService.svCodeAppli = "M" Then
            URL_SOCIETE = "/AM/Install.asp"
        ElseIf AUService.svCodeAppli = "V" Then
            URL_SOCIETE = "/AV/Install.asp"
        ElseIf AUService.svCodeAppli = "C" Then
            URL_SOCIETE = "/AC/Install.asp"
        ElseIf AUService.svCodeAppli = "B" Then
            URL_SOCIETE = "/BA/Install.asp"
        ElseIf AUService.svCodeAppli = "P" Then
            URL_SOCIETE = "/AP/Install.asp"
        ElseIf AUService.svCodeAppli = "D" Then
            URL_SOCIETE = "/MD/Install.asp"
        ElseIf AUService.svCodeAppli = "L" Then
            URL_SOCIETE = "/M2/Install.asp"
        ElseIf AUService.svCodeAppli = "S" Then
            URL_SOCIETE = "/P2/Install.asp"
        ElseIf AUService.svCodeAppli = "F" Then
            URL_SOCIETE = "/FV/Install.asp"
        ElseIf AUService.svCodeAppli = "G" Then
            URL_SOCIETE = "/IG/Install.asp"
        ElseIf AUService.svCodeAppli = "O" Then
            URL_SOCIETE = "/CP/Install.asp"
        ElseIf AUService.svCodeAppli = "U" Then
            URL_SOCIETE = "/RP/Install.asp"
        ElseIf AUService.svCodeAppli = "H" Then
            URL_SOCIETE = "/CC/Install.asp"
        Else
            Return ""
        End If

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        GetSociete = ""

        stUrl = ServerName + URL_SOCIETE + "?code=" + svSerialFound + "&serie=" + svSerial

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        If Status(lgInternet) Then
            GetSociete = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function InitializeUpdate(ByVal svString As String, Optional ByVal control As Boolean = False) As Boolean

        Dim bOpenInternet As Boolean

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        GetFileToRootDir(AUService.AppPath + AUService.DisplayNameReStart + ".exe", control)

        Dim lgInternet As Integer
        Dim stUrl As String = ServerName + "/fret2/update/"

        InitializeUpdate = False

        stUrl = stUrl.Replace(" ", "%20")

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        LocalFile = Application.ExecutablePath

        Dim AUServ As String = IO.Path.GetFileNameWithoutExtension(LocalFile)

        lgInternet = InternetOpenUrl(hInternet, stUrl + AUServ + ".Update" + CStr(IIf(control, "Test", "")), vbNullString, 0, InternetFlagsMask, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        Dim AccesUpdate As String = Nothing
        If Status(lgInternet) Then
            AccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If AccesUpdate <> "" Then

            If InStr(AccesUpdate, AUService.EndOfFile, CompareMethod.Text) > 0 Then

                Dim Update() As String = Split(AccesUpdate, ";")

                If CLng(Update(1)) < FileLen(LocalFile) Then Return False

                Dim GetAppBuild As String = Update(0)

                If StrCompTextVersions(GetAppBuild, svString) <> 0 Then

                    If Update.Length >= 4 Then

                        Dim nvResult As Integer = MsgBoxResult.No

                        If UCase(Update(3)).Trim = "VIRTUAL" And MacAddress.bVirtual Then
                            nvResult = MsgBox(mLanguageAU.GetString(MSG_UPDATE_SYSTEM_UPDATE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title)

                        ElseIf UCase(Update(3)).Trim = "ALL" Then
                            nvResult = MsgBox(mLanguageAU.GetString(MSG_UPDATE_SYSTEM_UPDATE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title)

                        Else
                            nvResult = MsgBoxResult.Yes

                        End If


                        If nvResult = MsgBoxResult.No Then

                            If bOpenInternet Then
                                Disconnect()
                            End If

                            Return InitializeUpdate

                        End If

                    End If

                    CheckSum = Update(2)
                    RemoteSize = CDbl(Update(1))

                    stUrl = stUrl.Replace(" ", "%20")
                    lgInternet = InternetOpenUrl(hInternet, stUrl + AUServ + "." + CheckSum, vbNullString, 0, InternetFlagsMask, 0)

                    If lgInternet = 0 Then
                        Exit Function
                    End If

                    Dim AppPath As String = IO.Path.GetDirectoryName(LocalFile)
                    If Not IsDirectoryWritable(AppPath) Then
                        MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + AppPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
                    End If

                    Dim AUServOld As String = IO.Path.ChangeExtension(LocalFile, ".backup")
                    Dim AUServNew As String = IO.Path.ChangeExtension(LocalFile, ".update")
                    Dim AUServExe As String = IO.Path.ChangeExtension(LocalFile, ".exe")

                    If IO.File.Exists(AUServOld) Then
                        Try
                            IO.File.Delete(AUServOld)

                        Catch ex As Exception

                            MsgBox("Delete file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)

                            InternetCloseHandle(lgInternet)

                            If bOpenInternet Then
                                Disconnect()
                            End If

                            Return False

                        End Try

                    End If

                    If IO.File.Exists(AUServNew) Then
                        Try
                            IO.File.Delete(AUServNew)

                        Catch ex As Exception

                            MsgBox("Delete file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)

                            InternetCloseHandle(lgInternet)

                            If bOpenInternet Then
                                Disconnect()
                            End If

                            Return False

                        End Try

                    End If

                    If IO.File.Exists(LocalFile) Then
                        Try
                            IO.File.Copy(AUServExe, AUServOld)

                        Catch ex As Exception
                            MsgBox("backup file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)

                            InternetCloseHandle(lgInternet)

                            If bOpenInternet Then
                                Disconnect()
                            End If

                            Return False
                        End Try

                    End If

                    If Not IO.File.Exists(AUServNew) Then

                        LocalFile = AUServNew

                        If Status(lgInternet) Then
                            ReadDataToFile(lgInternet, False)
                        End If

                        InternetCloseHandle(lgInternet)

                        If Not IO.File.Exists(TF(LocalFile)) Then
                            Return False

                        ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then
                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception

                                MsgBox("Delete temporary file (SIZE):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                                Return False

                            End Try


                        ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then

                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception
                                MsgBox("Delete temporary file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                        Else

                            Try
                                IO.File.Move(TF(LocalFile), LocalFile)
                            Catch ex As Exception
                                MsgBox("Install file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            InitializeUpdate = True

                            AUService.HttpSendMessage(CODE_UPDATE_AU_SERV, , , "Updating", svString, Application.ProductName)

                        End If

                    End If

                End If
            End If
        End If

        If bOpenInternet Then
            Disconnect()
        End If

        Return InitializeUpdate

    End Function

    Public Function GetFileAcces() As String

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        stUrl = ServerName + RemoteDir

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        LocalFile = Application.ExecutablePath
        Dim AUServ As String = IO.Path.GetFileNameWithoutExtension(LocalFile)

        lgInternet = InternetOpenUrl(hInternet, stUrl + AUService.svFileMaj, vbNullString, 0, InternetFlagsMask, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Return Nothing
        End If

        System.Windows.Forms.Application.DoEvents()

        Dim AccesUpdate As String = Nothing
        If Status(lgInternet) Then
            AccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

        Return AccesUpdate

    End Function

    Public Function GetProductUpdate(ByVal ProductCode As String) As String

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        stUrl = ServerName + RemoteDir

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        LocalFile = Application.ExecutablePath
        Dim AUServ As String = IO.Path.GetFileNameWithoutExtension(LocalFile)

        lgInternet = InternetOpenUrl(hInternet, stUrl + ProductCode, vbNullString, 0, InternetFlagsMask, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Return Nothing
        End If

        System.Windows.Forms.Application.DoEvents()

        Dim AccesUpdate As String = Nothing
        If Status(lgInternet) Then
            AccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

        Return AccesUpdate

    End Function

    Public Function GetLisenseAutorisation(ByVal szCompany As String, ByVal szHostName As String) As Boolean

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        Try

            stUrl = ServerName + "/fret2/license/" + AUService.svCodeAppli + "_" + szCompany + ".lic"

            stUrl = stUrl.Replace(" ", "%20")

            If DebugMode Then
                MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
            End If

            If hInternet = 0 Then
                bOpenInternet = OpenInternet()
            End If

            LocalFile = Application.ExecutablePath
            Dim AUServ As String = IO.Path.GetFileNameWithoutExtension(LocalFile)

            lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)

            If DebugMode Then
                MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
            End If

            If lgInternet = 0 Then Return True

            System.Windows.Forms.Application.DoEvents()

            Dim AccesUpdate As String = Nothing
            If Status(lgInternet) Then
                AccesUpdate = ReadDataToString(lgInternet)
            End If

            InternetCloseHandle(lgInternet)

            If bOpenInternet Then
                Disconnect()
            End If

            If AccesUpdate <> Nothing Then

                Dim licenselist As Array = Split(AccesUpdate, vbNewLine)

                If licenselist.Length > 0 Then

                    For Each licensecompare As String In licenselist
                        licensecompare = licensecompare.Trim
                        If licensecompare <> "" Then
                            If Right(licensecompare, 1) = "*" Then
                                If InStr(szHostName, Mid(licensecompare, 1, Len(licensecompare) - 1), CompareMethod.Text) = 1 Then Return True
                            ElseIf Left(licensecompare, 1) = "*" Then
                                If InStr(szHostName, Mid(licensecompare, 2, Len(licensecompare) - 1), CompareMethod.Text) > 1 Then Return True
                            ElseIf StrComp(szHostName, licensecompare, CompareMethod.Text) = 0 Then
                                Return True
                            End If
                        End If
                    Next

                    Return False

                End If
            Else
                Return True
            End If

        Catch ex As Exception

            Return True

        End Try


    End Function

    Public Function GetRemoteDir() As String

        If AUService.svCodeAppli = "R" Then
            Return "/fret2/radio/"

        ElseIf AUService.svCodeAppli = "I" Then
            Return "/fret2/Internet2/"

        ElseIf AUService.svCodeAppli = "A" Then
            Return "/fret2/Internet2/"

        ElseIf AUService.svCodeAppli = "M" Then
            Return "/fret2/mobile/"

        ElseIf AUService.svCodeAppli = "V" Then
            Return "/fret2/video/"

        ElseIf AUService.svCodeAppli = "C" Then
            Return "/fret2/courrier/"

        ElseIf AUService.svCodeAppli = "O" Then
            Return "/fret2/courrierplus/"

        ElseIf AUService.svCodeAppli = "D" Then
            Return "/fret2/metridom/"

        ElseIf AUService.svCodeAppli = "L" Then
            Return "/fret2/medial/"

        ElseIf AUService.svCodeAppli = "P" Then
            Return "/fret2/presse/"

        ElseIf AUService.svCodeAppli = "S" Then
            Return "/fret2/Presse2/"

        ElseIf AUService.svCodeAppli = "U" Then
            Return "/fret2/radioplus/"

        ElseIf AUService.svCodeAppli = "F" Then
            Return "/fret2/internetvideo/"

        ElseIf AUService.svCodeAppli = "H" Then
            Return "/fret2/choices/"

        Else
            Return Nothing
        End If

    End Function

    Public Function GetFileToRootDir(ByVal DlLocalFile As String, Optional ByVal test As Boolean = False) As Boolean

        Dim lgInternet As Integer
        Dim stUrl As String = ServerName + "/fret2/update/"
        Dim bOpenInternet As Boolean

        LocalFile = DlLocalFile

        GetFileToRootDir = False

        stUrl = stUrl.Replace(" ", "%20")

        Dim AppBuild As String = Nothing
        If IO.File.Exists(LocalFile) Then
            AppBuild = FileVersionInfo.GetVersionInfo(LocalFile).FileVersion
            If AppBuild = Nothing Then AppBuild = FileVersionInfo.GetVersionInfo(LocalFile).FileMajorPart.ToString + "." + FileVersionInfo.GetVersionInfo(LocalFile).FileMinorPart.ToString + "." + FileVersionInfo.GetVersionInfo(LocalFile).FileBuildPart.ToString
        End If

        If DebugMode Then
            MsgBox("InternetOpenUrl=" & stUrl, MsgBoxStyle.Information)
        End If

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        Dim DlFileToRoot As String = IO.Path.GetFileNameWithoutExtension(LocalFile)

        lgInternet = InternetOpenUrl(hInternet, stUrl + DlFileToRoot + ".Update" + CStr(IIf(test, "Test", "")), vbNullString, 0, InternetFlagsMask, 0)

        If DebugMode Then
            MsgBox("lgInternet=" & CStr(lgInternet), MsgBoxStyle.Information)
        End If

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        Dim DlFileAccesUpdate As String = Nothing
        If Status(lgInternet) Then
            DlFileAccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        If DlFileAccesUpdate <> "" Then

            If InStr(DlFileAccesUpdate, AUService.EndOfFile, CompareMethod.Text) > 0 Then

                Dim Update() As String = Split(DlFileAccesUpdate, ";")

                Dim GetAppBuild As String = Update(0)

                If StrCompTextVersions(GetAppBuild, AppBuild) > 0 Then

                    AUService.Hide()
                    PleaseWait.Show()

                    PleaseWait.ProgressBarPleaseWait.Value = 0
                    System.Windows.Forms.Application.DoEvents()

                    CheckSum = Update(2)
                    RemoteSize = CDbl(Update(1))

                    stUrl = stUrl.Replace(" ", "%20")

                    lgInternet = InternetOpenUrl(hInternet, stUrl + DlFileToRoot + "." + CheckSum, vbNullString, 0, InternetFlagsMask, 0)

                    System.Windows.Forms.Application.DoEvents()

                    If lgInternet = 0 Then

                        PleaseWait.Hide()
                        AUService.Show()
                        System.Windows.Forms.Application.DoEvents()

                        Exit Function

                    End If

                    If IO.File.Exists(LocalFile) Then

                        Try
                            IO.File.Delete(LocalFile)

                        Catch ex As Exception

                            InternetCloseHandle(lgInternet)

                            If bOpenInternet Then
                                Disconnect()
                            End If

                            PleaseWait.Hide()
                            AUService.Show()
                            System.Windows.Forms.Application.DoEvents()

                            Return False

                        End Try

                    End If

                    If Status(lgInternet) Then
                        ReadDataToFile(lgInternet, False, True)
                    End If

                    InternetCloseHandle(lgInternet)

                    If Not IO.File.Exists(TF(LocalFile)) Then

                        PleaseWait.Hide()
                        AUService.Show()
                        System.Windows.Forms.Application.DoEvents()

                        Exit Function

                    ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then
                        IO.File.Delete(TF(LocalFile))

                    ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then
                        IO.File.Delete(TF(LocalFile))

                    Else
                        IO.File.Move(TF(LocalFile), LocalFile)
                        GetFileToRootDir = True

                        AUService.HttpSendMessage(CODE_UPDATE_AU_SERV, , , "Updated", GetAppBuild, DlFileToRoot)

                    End If

                End If

            End If
        End If

        If bOpenInternet Then
            Disconnect()
        End If

        PleaseWait.Hide()
        AUService.Show()
        System.Windows.Forms.Application.DoEvents()

        Return GetFileToRootDir

    End Function

End Class
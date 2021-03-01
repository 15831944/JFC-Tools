Option Strict On
Option Explicit On

Public Class CConnection


    Private Declare Function InternetOpen Lib "wininet.dll" Alias "InternetOpenA" (ByVal sAgent As String, ByVal lAccessType As Integer, ByVal sProxyName As String, ByVal sProxyBypass As String, ByVal lFlags As Integer) As Integer
    Private Declare Function HttpQueryInfo Lib "wininet.dll" Alias "HttpQueryInfoA" (ByVal hHttpRequest As Integer, ByVal lInfoLevel As Integer, ByVal sBuffer As String, ByRef lBufferLength As Integer, ByRef lIndex As Integer) As Boolean
    Private Declare Function HttpSendRequest Lib "wininet.dll" Alias "HttpSendRequestA" (ByVal hHttpRequest As Integer, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal sOptional As String, ByVal lOptionalLength As Integer) As Boolean
    Private Declare Function InternetCloseHandle Lib "wininet.dll" (ByVal hInet As Integer) As Boolean
    Private Declare Function InternetOpenUrl Lib "wininet.dll" Alias "InternetOpenUrlA" (ByVal hInternetSession As Integer, ByVal sUrl As String, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal lFlags As Integer, ByVal lContext As Long) As Integer
    Private Declare Function InternetReadFile Lib "wininet.dll" (ByVal hFile As Integer, ByVal sBuffer As String, ByVal lNumBytesToRead As Integer, ByRef lNumberOfBytesRead As Integer) As Boolean
    Private Declare Function InternetSetOption Lib "wininet.dll" Alias "InternetSetOptionA" (ByVal hInternet As Integer, ByVal dwOption As Integer, ByVal lpBuffer As String, ByVal dwBufferLength As Integer) As Boolean
    Private Declare Function InternetConnect Lib "wininet.dll" Alias "InternetConnectA" (ByVal hInternetSession As Integer, ByVal sServerName As String, ByVal nServerPort As Integer, ByVal sUserName As String, ByVal sPassword As String, ByVal lService As Integer, ByVal lFlags As Integer, ByVal lContext As Integer) As Integer

    Public hInternet As Integer
    Public hConnection As Integer
    Public UserName As String
    Public Password As String
    Public RemoteFile As String
    Public LocalFile As String
    Public RemoteSize As Double
    Public Proxy As String
    Public ProxyLog As String
    Public ProxyPwd As String
    Public DirectConnect As Boolean
    Public CheckSum As String
    Public InternetBuffer As Integer
    Public InternetFlagsMask As Integer

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

    Const INTERNET_FLAG_EXISTING_CONNECT As Integer = &H20000000  ' do not create new connection object
    Const INTERNET_FLAG_RELOAD As Integer = &H80000000  ' retrieve the original item

    Public Const INTERNET_FLAGS_MASK As Integer = INTERNET_FLAG_RELOAD Or INTERNET_FLAG_EXISTING_CONNECT ' &HA0000000

    Public Sub InitHTTP()

        Dim AUServIni As String = MultiUserEDI.AppPath + "AU-Serv.ini"

        If IO.File.Exists(AUServIni) Then

            UserName = Select_GetIniString("Parametres", "UserName", AUServIni).Trim
            Password = Select_GetIniString("Parametres", "Password", AUServIni).Trim
            DirectConnect = (Select_GetIniString("Parametres", "DirectConnect", AUServIni).Trim = "1")

            Dim nBuffer As Integer = CInt(Val(Select_GetIniString("Parametres", "Buffer", AUServIni)))
            If nBuffer = 0 Then nBuffer = 8

            InternetBuffer = nBuffer

            InternetFlagsMask = CInt(Val(Select_GetIniString("Parametres", "InternetFlagsMask", AUServIni)))

            ' Définition du Proxy
            ProxyLog = Nothing
            ProxyPwd = Nothing
            Proxy = Select_GetIniString("Parametres", "Proxy", AUServIni).Trim
            If Proxy <> "" Then
                Dim nPosProxy As Short = CShort(InStr(Proxy, "@"))
                If nPosProxy > 0 Then
                    Dim nPosPwd As Short = CShort(InStr(Proxy, ":"))
                    ProxyLog = Mid(Proxy, 1, nPosPwd - 1)
                    ProxyPwd = Mid(Proxy, nPosPwd + 1, nPosProxy - nPosPwd - 1)
                    Proxy = Mid(Proxy, nPosProxy + 1, Len(Proxy) - nPosProxy)
                End If
                Proxy = "http=http://" & Proxy
            End If
        Else

            InternetBuffer = 8

        End If

    End Sub

    Public Function InternetOpenHandle(Optional ByRef Proxy As String = vbNullString) As Integer

        InternetOpenHandle = InternetOpen(scUserAgent,
                      CInt(IIf((Proxy = ""),
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PRECONFIG),
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_PROXY, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY))),
                      Proxy,
                      CStr(IIf((Proxy = ""), vbNullString, "<local>")),
                      0)

    End Function

    Public Function OpenInternet() As Boolean

        OpenInternet = False

        If InternetFlagsMask = 0 Then
            InternetFlagsMask = INTERNET_FLAGS_MASK
        End If

        'Ouverture d'une connexion Internet global si pas de Proxy
        If Proxy = vbNullString Then
            hInternet = InternetOpenHandle()
        Else
            hInternet = InternetOpenHandle(Proxy)
        End If

        If hInternet = 0 Then
            Exit Function
        End If

        '********************************
        ' Test de connexion (08-03-2011)
        '********************************
        Dim lgInternet As Integer
        Dim stUrl As String = "https://mp.kantarmedia.fr/"

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, InternetFlagsMask, 0)
        If lgInternet <> 0 Then
            InternetCloseHandle(lgInternet)
        End If
        '********************************

        'Retourne Vrai si la connexion est établie
        OpenInternet = (lgInternet <> 0) '(hInternet <> 0)

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

    Private Sub ReadDataToFile(ByVal hOpen As Integer)

        Dim sBuffer As String
        Dim ret As Integer
        Dim BytesBuff As Integer
        Dim SentBytes As Double
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

        Try
            FileOpen(FF, TF(LocalFile), OpenMode.Binary, OpenAccess.Write, OpenShare.LockReadWrite)
        Catch ex As Exception
            MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", Chr(34) + TF(LocalFile) + Chr(34)) & vbCrLf & ex.Message, MsgBoxStyle.Critical, My.Application.Info.Title)
            InternetCloseHandle(hOpen)
            Exit Sub
        End Try

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

            SentBytes += ret

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

        Dim lgRet As Boolean = InternetSetOption(hOpen, INTERNET_OPTION_USERNAME, UserName, Len(UserName))

        If lgRet = False Then
            Return False
        End If

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PASSWORD, Password, Len(Password))

        Return lgRet

    End Function

    Private Function SetOptionTimeOut(ByVal hOpen As Integer) As Boolean

        Dim conntimeout As Integer = 1000 '(en ms) ici j'ai fait divers essais pour augmenter ou diminuer rien n'y fait

        SetOptionTimeOut = InternetSetOption(hOpen, INTERNET_OPTION_CONNECT_TIMEOUT, CStr(Int(conntimeout)), Len(conntimeout))

    End Function

    Private Function SetOptionProxy(ByVal hOpen As Integer) As Boolean

        Dim lgRet As Boolean = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_USERNAME, ProxyLog, Len(ProxyLog))

        If lgRet = False Then
            Return False
        End If

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_PASSWORD, ProxyPwd, Len(ProxyPwd))

        Return lgRet

    End Function

    Public Function DeleteZ(ByVal sValue As String) As String
        Return Replace(sValue, "Z", "")
    End Function

    Public Function Status(ByRef hOpen As Integer) As Boolean

        Dim lgRet As Boolean
        Dim lgErr As Integer

        lgErr = GetErr(hOpen)

        System.Windows.Forms.Application.DoEvents()

        If lgErr = HTTP_STATUS_NOT_FOUND Then

            Status = False
            Exit Function

        End If

        If lgErr = HTTP_STATUS_PROXY_AUTH_REQ Then

            lgRet = SetOptionProxy(hOpen)

            lgRet = HttpSendRequest(hOpen, vbNullString, 0, vbNullString, 0)

            lgErr = GetErr(hOpen)

        End If

        If lgErr = HTTP_STATUS_DENIED Then

            lgRet = SetOption(hOpen)

            lgRet = HttpSendRequest(hOpen, vbNullString, 0, vbNullString, 0)

            lgErr = GetErr(hOpen)

        End If

        Status = (lgErr = HTTP_STATUS_OK)

    End Function

    Public Function TF(ByRef szFile As String) As String
        Return szFile + ".tmp"
    End Function

    Public Function GetNewUpdate(ByVal AppBuild As String) As Boolean

        Dim lgInternet As Integer
        Dim stUrl As String = "https://mp.kantarmedia.fr/fret2/MultiUser/"

        GetNewUpdate = False

        stUrl = stUrl.Replace(" ", "%20")

        LocalFile = Application.ExecutablePath
        Dim MultiUser As String = ParsePath(LocalFile, FILENAME_ONLY)

        lgInternet = InternetOpenUrl(hInternet, stUrl + MultiUser + ".Update", vbNullString, 0, InternetFlagsMask, 0)

        If lgInternet = 0 Then
            Exit Function
        End If

        System.Windows.Forms.Application.DoEvents()

        Dim AccesUpdate As String = Nothing
        If Status(lgInternet) Then
            AccesUpdate = ReadDataToString(lgInternet)
        End If

        InternetCloseHandle(lgInternet)

        'Ex: 5.8.0.2,3038425,B69CCD76##
        If AccesUpdate <> "" Then

            If InStr(AccesUpdate, "##", CompareMethod.Text) > 0 Then

                Dim Update() As String = Split(AccesUpdate, ";")

                Dim GetAppBuild As String = Update(0)

                If StrCompTextVersions(GetAppBuild, AppBuild) <> 0 Then

                    CheckSum = Update(2)
                    RemoteSize = CDbl(Update(1))

                    stUrl = stUrl.Replace(" ", "%20")
                    lgInternet = InternetOpenUrl(hInternet, stUrl + MultiUser + "." + CheckSum, vbNullString, 0, InternetFlagsMask, 0)

                    If lgInternet = 0 Then
                        Exit Function
                    End If

                    Dim AppPath As String = ParsePath(LocalFile, PATH)
                    If Not IsDirectoryWritable(AppPath) Then
                        MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", Chr(34) + AppPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
                    End If

                    Dim MultiUserPath As String = ParsePath(LocalFile, PATH_FILENAME_ONLY)
                    Dim MultiUserOld As String = MultiUserPath & ".old"
                    Dim MultiUserNew As String = MultiUserPath & ".new"
                    Dim MultiUserExe As String = MultiUserPath & ".exe"

                    If IO.File.Exists(MultiUserOld) Then
                        Try
                            IO.File.Delete(MultiUserOld)

                        Catch ex As Exception

                            MsgBox("Delete file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)

                            InternetCloseHandle(lgInternet)

                            Return False

                        End Try

                    End If

                    If IO.File.Exists(LocalFile) Then
                        Try
                            IO.File.Move(MultiUserExe, MultiUserOld)

                        Catch ex As Exception
                            MsgBox("backup file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)

                            InternetCloseHandle(lgInternet)

                            Return False
                        End Try

                    End If

                    If Not IO.File.Exists(MultiUserExe) Then

                        If Status(lgInternet) Then
                            ReadDataToFile(lgInternet)
                        End If

                        InternetCloseHandle(lgInternet)

                        If Not IO.File.Exists(TF(LocalFile)) Then
                            Return False
                            'Exit Function

                        ElseIf RemoteSize <> FileLen(TF(LocalFile)) Then
                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception

                                MsgBox("Delete temporary file (SIZE):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                                Return False

                            End Try

                            Try
                                IO.File.Move(MultiUserOld, MultiUserExe)
                            Catch ex As Exception

                                MsgBox("Restore file (SIZE):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                                Return False

                            End Try

                            'Error CRC
                        ElseIf Append(TF(LocalFile)) <> DeleteZ(CheckSum) Then

                            Try
                                IO.File.Delete(TF(LocalFile))
                            Catch ex As Exception
                                MsgBox("Delete temporary file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            Try
                                IO.File.Move(MultiUserOld, MultiUserExe)
                            Catch ex As Exception
                                MsgBox("Restore file (CRC):" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try


                        Else

                            Try
                                IO.File.Move(TF(LocalFile), LocalFile)
                            Catch ex As Exception
                                MsgBox("Install file:" + vbNewLine + ex.Message, MsgBoxStyle.Exclamation)
                            End Try

                            GetNewUpdate = True

                        End If

                    End If

                End If
            End If
        End If

        Return GetNewUpdate

    End Function

End Class
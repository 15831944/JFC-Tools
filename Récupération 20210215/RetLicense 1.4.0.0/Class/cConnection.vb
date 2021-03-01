Option Strict On
Option Explicit On

Imports System.Net
Imports System.IO
Imports System

Friend Class cConnection

    Private Declare Function HttpQueryInfo Lib "wininet.dll" Alias "HttpQueryInfoA" (ByVal hHttpRequest As Integer, ByVal lInfoLevel As Integer, ByVal sBuffer As String, ByRef lBufferLength As Integer, ByRef lIndex As Integer) As Boolean
    Private Declare Function HttpSendRequest Lib "wininet.dll" Alias "HttpSendRequestA" (ByVal hHttpRequest As Integer, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal sOptional As String, ByVal lOptionalLength As Integer) As Integer
    Private Declare Function InternetCloseHandle Lib "wininet" (ByVal hInet As Integer) As Short
    Private Declare Function InternetConnect Lib "wininet.dll" Alias "InternetConnectA" (ByVal hInternetSession As Integer, ByVal sServerName As String, ByVal nServerPort As Short, ByVal sUserName As String, ByVal sPassword As String, ByVal lService As Integer, ByVal lFlags As Integer, ByVal lContext As Integer) As Integer
    Private Declare Function InternetOpen Lib "wininet" Alias "InternetOpenA" (ByVal sAgent As String, ByVal lAccessType As Short, ByVal sProxyName As String, ByVal sProxyBypass As String, ByVal lFlags As Integer) As Integer
    Private Declare Function InternetOpenUrl Lib "wininet.dll" Alias "InternetOpenUrlA" (ByVal hInternetSession As Integer, ByVal sUrl As String, ByVal sHeaders As String, ByVal lHeadersLength As Integer, ByVal lFlags As Integer, ByVal lContext As Integer) As Integer
    Private Declare Function InternetReadFile Lib "wininet" (ByVal hFile As Integer, ByVal sBuffer As String, ByVal lNumBytesToRead As Integer, ByRef lNumberOfBytesRead As Integer) As Short
    Private Declare Function InternetSetOption Lib "wininet.dll" Alias "InternetSetOptionA" (ByVal hInternet As Integer, ByVal dwOption As Integer, ByVal lpBuffer As String, ByVal dwBufferLength As Integer) As Integer

    Public hInternet As Integer
    Public hConnection As Integer
    Public UserName As String
    Public Password As String
    Public Proxy As String
    Public ProxyLog As String
    Public ProxyPwd As String
    Public DirectConnect As Boolean
    Public InternetFlagsMask As Integer

    Const scUserAgent As String = "AU-Serv Connexion"

    Const INTERNET_OPEN_TYPE_PRECONFIG As Short = 0
    Const INTERNET_OPEN_TYPE_DIRECT As Short = 1
    Const INTERNET_OPEN_TYPE_PROXY As Short = 3
    Const INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY As Short = 4

    Const HTTP_QUERY_STATUS_CODE As Short = 19
    Const HTTP_STATUS_PROXY_AUTH_REQ As Short = 407
    Const HTTP_STATUS_OK As Short = 200
    Const HTTP_STATUS_DENIED As Short = 401
    Const HTTP_STATUS_NOT_FOUND As Short = 404

    Const INTERNET_OPTION_PASSWORD As Short = 29
    Const INTERNET_OPTION_USERNAME As Short = 28
    Const INTERNET_OPTION_PROXY As Short = 38
    Const INTERNET_OPTION_PROXY_USERNAME As Short = 43
    Const INTERNET_OPTION_PROXY_PASSWORD As Short = 44

    Const INTERNET_OPTION_CONNECT_TIMEOUT As Short = 2
    Const INTERNET_OPTION_RECEIVE_TIMEOUT As Short = 6
    Const INTERNET_OPTION_SEND_TIMEOUT As Short = 5
    Const INTERNET_OPTION_CONNECT_RETRIES As Short = 3
    Const INTERNET_OPTION_DATA_RECEIVE_TIMEOUT As Short = 8
    Const INTERNET_OPTION_SETTINGS_CHANGED As Short = 39
    Const INTERNET_OPTION_REFRESH As Short = 37

    Const INTERNET_FLAG_EXISTING_CONNECT As Integer = &H20000000  ' do not create new connection object
    Const INTERNET_FLAG_RELOAD As Integer = &H80000000  ' retrieve the original item
    
    Public Const INTERNET_FLAGS_MASK As Integer = INTERNET_FLAG_RELOAD Or INTERNET_FLAG_EXISTING_CONNECT ' &HA0000000


    Public Function InternetOpenHandle(Optional ByRef Proxy As String = "") As Integer

        InternetOpenHandle = InternetOpen(scUserAgent, _
                      CShort(IIf((Proxy = ""), _
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PRECONFIG), _
                          IIf(DirectConnect, INTERNET_OPEN_TYPE_PROXY, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY))), _
                      Proxy, _
                      CStr(IIf((Proxy = ""), vbNullString, "<local>")), _
                      0)

    End Function

    Public Function OpenInternet() As Boolean

        'Ouverture d'une connexion Internet global si pas de Proxy
        If Proxy = vbNullString Then
            hInternet = InternetOpenHandle()
        Else
            hInternet = InternetOpenHandle(Proxy)
        End If

        Return (hInternet <> 0)

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

        BytesBuff = 1024 * 8
        sBuffer = Space(BytesBuff)
        sRec = vbNullString

        Do
            If InternetReadFile(hOpen, sBuffer, BytesBuff, ret) = 0 Then
                Exit Do
            End If

            System.Windows.Forms.Application.DoEvents()

            sRec = sRec & Mid(sBuffer, 1, ret)
        Loop Until ret <> BytesBuff

        System.Windows.Forms.Application.DoEvents()

        ReadDataToString = sRec
        InternetCloseHandle(hOpen)

        System.Windows.Forms.Application.DoEvents()

    End Function

    Private Function SetOption(ByVal hOpen As Integer) As Integer

        Dim lgRet As Integer

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_USERNAME, UserName, Len(UserName))

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PASSWORD, Password, Len(Password))

        SetOption = lgRet

    End Function

    Private Function SetOptionTimeOut(ByVal hOpen As Integer) As Integer

        Dim conntimeout As Integer = 1000 '(en ms) ici j'ai fait divers essais pour augmenter ou diminuer rien n'y fait

        SetOptionTimeOut = InternetSetOption(hOpen, INTERNET_OPTION_CONNECT_TIMEOUT, CStr(Int(conntimeout)), Len(conntimeout))

    End Function

    Private Function SetOptionProxy(ByVal hOpen As Integer) As Integer

        Dim lgRet As Integer

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_USERNAME, ProxyLog, Len(ProxyLog))

        lgRet = InternetSetOption(hOpen, INTERNET_OPTION_PROXY_PASSWORD, ProxyPwd, Len(ProxyPwd))

        SetOptionProxy = lgRet

    End Function

    Public Function Status(ByRef hOpen As Integer) As Boolean

        Dim lgRet, lgErr As Integer

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

            'InternetConnect()
            'HttpOpenRequest()
            'HttpEndRequest()

            lgErr = GetErr(hOpen)
        End If

        Status = (lgErr = HTTP_STATUS_OK)

    End Function

    'http://www.jfc-infomedia.fr/ai/poste.asp?act=4&serie=WTM0TNW4SRJ9S8GQDPGI1SQMY&blq=1

    Public Function BloqueProductCode(ByVal svSerial As String, ByVal CodeApp As String) As Integer

        Dim lgInternet As Integer
        Dim stUrl As String = "http://www.jfc-infomedia.fr/" + CodeApp + "/poste.asp?act=4&serie=" + svSerial.Replace("-", "") + "&blq=1"
        Dim bOpenInternet As Boolean

        BloqueProductCode = 0

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        'If lgInternet = 0 Then
        '    Exit Function
        'End If

        'System.Windows.Forms.Application.DoEvents()

        'If Status(lgInternet) Then
        '    BloqueProductCode = CInt(ReadDataToString(lgInternet))
        'End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function UnlockProductCode(ByVal svSerial As String, ByVal CodeApp As String) As Integer

        Dim lgInternet As Integer
        Dim stUrl As String = "http://www.jfc-infomedia.fr/" + CodeApp + "/poste.asp?act=4&serie=" + svSerial.Replace("-", "") + "&blq=0"
        Dim bOpenInternet As Boolean

        UnlockProductCode = 0

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        'If lgInternet = 0 Then
        '    Exit Function
        'End If

        'System.Windows.Forms.Application.DoEvents()

        'If Status(lgInternet) Then
        '    BloqueProductCode = CInt(ReadDataToString(lgInternet))
        'End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function DeleteProductCode(ByVal svSerial As String, ByVal CodeApp As String) As Integer

        Dim lgInternet As Integer
        Dim stUrl As String = "http://www.jfc-infomedia.fr/" + CodeApp + "/poste.asp?act=1&serie=" + svSerial.Replace("-", "")
        Dim bOpenInternet As Boolean

        DeleteProductCode = 0

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

        'If lgInternet = 0 Then
        '    Exit Function
        'End If

        'System.Windows.Forms.Application.DoEvents()

        'If Status(lgInternet) Then
        '    BloqueProductCode = CInt(ReadDataToString(lgInternet))
        'End If

        InternetCloseHandle(lgInternet)

        If bOpenInternet Then
            Disconnect()
        End If

    End Function

    Public Function ReturnProductCode(ByVal svSerial As String, ByVal CodeApp As String) As Integer

        Dim URL_AUTORISATION As String = ""

        URL_AUTORISATION = "http://www.jfc-infomedia.fr/" + CodeApp + "/uninstall.asp"

        Dim lgInternet As Integer
        Dim stUrl As String
        Dim bOpenInternet As Boolean

        ReturnProductCode = 0

        stUrl = URL_AUTORISATION + "?serie=" + svSerial.Replace("-", "")

        If hInternet = 0 Then
            bOpenInternet = OpenInternet()
        End If

        lgInternet = InternetOpenUrl(hInternet, stUrl, vbNullString, 0, INTERNET_FLAGS_MASK, 0)

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

End Class
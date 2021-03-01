Option Strict On
Option Explicit On

Public Class Form1

    Const INTERNET_FLAG_SECURE As Long = &H800000  ' use PCT/SSL if applicable (HTTP)
    Const INTERNET_FLAG_RAW_DATA As Long = &H40000000  ' receive the item as raw data
    Const INTERNET_FLAG_EXISTING_CONNECT As Long = &H20000000  ' do not create new connection object
    Const INTERNET_FLAG_RELOAD As Long = 2147483648 '&H80000000 'retrieve the original item
    Const INTERNET_FLAG_KEEP_CONNECTION As Long = &H400000  ' use keep-alive semantics
    Const INTERNET_FLAG_NO_AUTO_REDIRECT As Long = &H200000  ' don't handle redirections automatically
    Const INTERNET_FLAG_READ_PREFETCH As Long = &H100000  ' do background read prefetch
    Const INTERNET_FLAG_NO_COOKIES As Long = &H80000  ' no automatic cookie handling
    Const INTERNET_FLAG_NO_AUTH As Long = &H40000  ' no automatic authentication handling
    Const INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP As Long = &H8000 ' ex: https:// to http://
    Const INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS As Long = &H4000 ' ex: http:// to https://
    Const INTERNET_FLAG_IGNORE_CERT_DATE_INVALID As Long = &H2000 ' expired X509 Cert.
    Const INTERNET_FLAG_IGNORE_CERT_CN_INVALID As Long = &H1000 ' bad common name in X509 Cert.
    Const INTERNET_FLAG_MUST_CACHE_REQUEST As Long = &H10 ' fails if unable to cache request
    Const INTERNET_FLAG_RESYNCHRONIZE As Long = &H800 ' asking wininet to update an item if it is newer
    Const INTERNET_FLAG_HYPERLINK As Long = &H400 ' asking wininet to do hyperlinking semantic which works right for scripts
    Const INTERNET_FLAG_NO_UI As Long = &H200
    Const INTERNET_FLAG_ASYNC As Long = &H10000000  ' this request is asynchronous (where supported)
    Const INTERNET_FLAG_PASSIVE As Long = &H8000000  ' used for FTP connections
    Const INTERNET_FLAG_NO_CACHE_WRITE As Long = &H4000000  ' don't write this item to the cache
    Const INTERNET_FLAG_DONT_CACHE As Long = INTERNET_FLAG_NO_CACHE_WRITE
    Const INTERNET_FLAG_MAKE_PERSISTENT As Long = &H2000000  ' make this item persistent in cache
    Const INTERNET_FLAG_OFFLINE As Long = &H1000000  ' use offline semantics

    Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal MyApplicationName As String, ByVal MyKeyName As String, ByVal MykeyString As String, ByVal MyFileName As String) As Short
    Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal NomModule As String, ByVal MotCle As String, ByVal StringParDefaut As String, ByVal ReturnedString As String, ByVal Longueur As Short, ByVal NomFichierIni As String) As Short

    Private Const MAX_LEN As Short = 640 '260
    Private bStartProg As Boolean = True

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        CheckBox1.Text = "INTERNET_FLAG_SECURE As Integer = &H800000  ' use PCT/SSL if applicable (HTTP)"
        CheckBox2.Text = "INTERNET_FLAG_RAW_DATA As Integer = &H40000000  ' receive the item as raw data"
        CheckBox3.Text = "INTERNET_FLAG_EXISTING_CONNECT As Long = &H20000000  ' do not create new connection object"
        CheckBox4.Text = "INTERNET_FLAG_RELOAD As Integer = &H80000000  ' retrieve the original item"
        CheckBox5.Text = "INTERNET_FLAG_KEEP_CONNECTION As Integer = &H400000  ' use keep-alive semantics"
        CheckBox6.Text = "INTERNET_FLAG_NO_AUTO_REDIRECT As Integer = &H200000  ' don't handle redirections automatically"
        CheckBox7.Text = "INTERNET_FLAG_READ_PREFETCH As Integer = &H100000  ' do background read prefetch"
        CheckBox8.Text = "INTERNET_FLAG_NO_COOKIES As Integer = &H80000  ' no automatic cookie handling"
        CheckBox9.Text = "INTERNET_FLAG_NO_AUTH As Integer = &H40000  ' no automatic authentication handling"
        CheckBox10.Text = "INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP As Integer = &H8000 ' ex: https:// to http://"
        CheckBox11.Text = "INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS As Integer = &H4000 ' ex: http:// to https://"
        CheckBox12.Text = "INTERNET_FLAG_IGNORE_CERT_DATE_INVALID As Integer = &H2000 ' expired X509 Cert."
        CheckBox13.Text = "INTERNET_FLAG_IGNORE_CERT_CN_INVALID As Integer = &H1000 ' bad common name in X509 Cert."
        CheckBox14.Text = "INTERNET_FLAG_MUST_CACHE_REQUEST As Integer = &H10 ' fails if unable to cache request"
        CheckBox15.Text = "INTERNET_FLAG_RESYNCHRONIZE As Integer = &H800 ' asking wininet to update an item if it is newer"
        CheckBox16.Text = "INTERNET_FLAG_HYPERLINK As Integer = &H400 ' asking wininet to do hyperlinking semantic which works right for scripts"
        CheckBox17.Text = "INTERNET_FLAG_NO_UI As Integer = &H200"
        CheckBox18.Text = "INTERNET_FLAG_ASYNC As Integer = &H10000000  ' this request is asynchronous (where supported)"
        CheckBox19.Text = "INTERNET_FLAG_PASSIVE As Integer = &H8000000  ' used for FTP connections"
        CheckBox20.Text = "INTERNET_FLAG_NO_CACHE_WRITE As Integer = &H4000000  ' don't write this item to the cache"
        CheckBox21.Text = "INTERNET_FLAG_DONT_CACHE As Integer = INTERNET_FLAG_NO_CACHE_WRITE"
        CheckBox22.Text = "INTERNET_FLAG_MAKE_PERSISTENT As Integer = &H2000000  ' make this item persistent in cache"
        CheckBox23.Text = "INTERNET_FLAG_OFFLINE As Integer = &H1000000  ' use offline semantics"

        TextBox1.Text = "0"

        Me.Show()

        CheckIni()

        bStartProg = False

    End Sub

    Private Sub ClickBox()

        If bStartProg = True Then Exit Sub

        Dim INTERNET_FLAGS_MASK As Long = 0

        If CheckBox1.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_SECURE
        If CheckBox2.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_RAW_DATA
        If CheckBox3.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_EXISTING_CONNECT
        If CheckBox4.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_RELOAD
        If CheckBox5.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_KEEP_CONNECTION
        If CheckBox6.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_NO_AUTO_REDIRECT
        If CheckBox7.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_READ_PREFETCH
        If CheckBox8.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_NO_COOKIES
        If CheckBox9.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_NO_AUTH
        If CheckBox10.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP
        If CheckBox11.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS
        If CheckBox12.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
        If CheckBox13.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_IGNORE_CERT_CN_INVALID
        If CheckBox14.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_MUST_CACHE_REQUEST
        If CheckBox15.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_RESYNCHRONIZE
        If CheckBox16.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_HYPERLINK
        If CheckBox17.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_NO_UI
        If CheckBox18.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_ASYNC
        If CheckBox19.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_PASSIVE
        If CheckBox20.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_NO_CACHE_WRITE
        If CheckBox21.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_DONT_CACHE
        If CheckBox22.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_MAKE_PERSISTENT
        If CheckBox23.Checked Then INTERNET_FLAGS_MASK += INTERNET_FLAG_OFFLINE

        ''If INTERNET_FLAGS_MASK = 0 Then
        ''    CheckBox3.Checked = True '(INTERNET_FLAGS_MASK = 0)
        ''    INTERNET_FLAGS_MASK = 2147483648
        ''End If

        Dim Val As String = INTERNET_FLAGS_MASK.ToString ' "&H" & Trim(Hex(CLng(INTERNET_FLAGS_MASK))).Replace("FF", "")

        TextBox1.Text = Val

        If INTERNET_FLAGS_MASK = 0 Or INTERNET_FLAGS_MASK = INTERNET_FLAG_RELOAD + INTERNET_FLAG_EXISTING_CONNECT Then
            Select_WriteIniString("Parametres", "InternetFlagsMask", "", Application.StartupPath & "\AU-Serv.ini")
        Else
            Select_WriteIniString("Parametres", "InternetFlagsMask", Trim(TextBox1.Text), Application.StartupPath & "\AU-Serv.ini")
        End If



    End Sub



    Private Function Check(ByRef m As Long, ByVal n As Long) As Boolean

        Dim q As Long = (n Xor m)
        If n >= q Then
            Check = (n - q = m)
        Else
            Check = False
        End If
    End Function

    Private Sub CheckIni()

        Dim j As String = Select_GetIniString("Parametres", "InternetFlagsMask", Application.StartupPath & "\AU-Serv.ini")

        If j = "" Then j = "&HA0000000"

        Dim d As Long

        If j.ToUpper.Contains("&H") Then
            d = Convert.ToInt64(j.Replace("&H", ""), 16)
        Else
            d = CLng(j)
        End If

        TextBox1.Text = d.ToString '"&H" & Trim(Hex(d))

        CheckBox4.CheckState = CType(IIf(Check(INTERNET_FLAG_RELOAD, d), CheckState.Checked, CheckState.Unchecked), CheckState)

        CheckBox1.CheckState = CType(IIf(Check(INTERNET_FLAG_SECURE, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox2.CheckState = CType(IIf(Check(INTERNET_FLAG_RAW_DATA, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox3.CheckState = CType(IIf(Check(INTERNET_FLAG_EXISTING_CONNECT, d), CheckState.Checked, CheckState.Unchecked), CheckState)

        CheckBox5.CheckState = CType(IIf(Check(INTERNET_FLAG_KEEP_CONNECTION, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox6.CheckState = CType(IIf(Check(INTERNET_FLAG_NO_AUTO_REDIRECT, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox7.CheckState = CType(IIf(Check(INTERNET_FLAG_READ_PREFETCH, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox8.CheckState = CType(IIf(Check(INTERNET_FLAG_NO_COOKIES, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox9.CheckState = CType(IIf(Check(INTERNET_FLAG_NO_AUTH, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox10.CheckState = CType(IIf(Check(INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox11.CheckState = CType(IIf(Check(INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox12.CheckState = CType(IIf(Check(INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox13.CheckState = CType(IIf(Check(INTERNET_FLAG_IGNORE_CERT_CN_INVALID, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox14.CheckState = CType(IIf(Check(INTERNET_FLAG_MUST_CACHE_REQUEST, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox15.CheckState = CType(IIf(Check(INTERNET_FLAG_RESYNCHRONIZE, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox16.CheckState = CType(IIf(Check(INTERNET_FLAG_HYPERLINK, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox17.CheckState = CType(IIf(Check(INTERNET_FLAG_NO_UI, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox18.CheckState = CType(IIf(Check(INTERNET_FLAG_ASYNC, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox19.CheckState = CType(IIf(Check(INTERNET_FLAG_PASSIVE, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox20.CheckState = CType(IIf(Check(INTERNET_FLAG_NO_CACHE_WRITE, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox21.CheckState = CType(IIf(Check(INTERNET_FLAG_DONT_CACHE, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox22.CheckState = CType(IIf(Check(INTERNET_FLAG_MAKE_PERSISTENT, d), CheckState.Checked, CheckState.Unchecked), CheckState)
        CheckBox23.CheckState = CType(IIf(Check(INTERNET_FLAG_OFFLINE, d), CheckState.Checked, CheckState.Unchecked), CheckState)


    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        End
    End Sub

    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click

        Dim ExecuteFile As String = Application.StartupPath & "\AU-Serv.exe"

        If IO.File.Exists(ExecuteFile) Then
            Me.Hide()
            LaunchAppAndWait(ExecuteFile, WAIT)
            Me.Show()
        End If

    End Sub

    Private Sub CheckBox1_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox1.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox2_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox2.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox3_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox3.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox4_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox4.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox5_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox5.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox6_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox6.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox7_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox7.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox8_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox8.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox9_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox9.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox10_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox10.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox11_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox11.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox12_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox12.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox13_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox13.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox14_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox14.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox15_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox15.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox16_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox16.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox17_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox17.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox18_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox18.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox19_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox19.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox20_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox20.CheckedChanged
        ClickBox()
        CheckBox21.CheckState = CType(IIf(CheckBox20.CheckState = CheckState.Checked, CheckState.Checked, CheckState.Unchecked), CheckState)
    End Sub

    Private Sub CheckBox21_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox21.CheckedChanged
        ClickBox()
        CheckBox20.CheckState = CType(IIf(CheckBox21.CheckState = CheckState.Checked, CheckState.Checked, CheckState.Unchecked), CheckState)
    End Sub

    Private Sub CheckBox22_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox22.CheckedChanged
        ClickBox()
    End Sub

    Private Sub CheckBox24_CheckedChanged(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        For Each cc As Control In Me.Controls
            If TypeOf cc Is CheckBox Then
                DirectCast(cc, CheckBox).Checked = False
            End If
        Next
    End Sub

    Public Sub Select_WriteIniString(ByVal NomModule As String, ByVal MotClé As String, ByVal Chaine As String, ByVal FichierIni As String)

        'Modif qui supprime la ligne
        If Chaine = "" Then Chaine = vbNullString

        Dim Status As Short
        Status = WritePrivateProfileString(NomModule, MotClé, Chaine, FichierIni)

    End Sub

    Public Function Select_GetIniString(ByRef NomModule As String, ByRef MotCle As String, ByRef FichierIni As String) As String

        Dim Chaine As String
        Dim Status As Short

        Chaine = Space(MAX_LEN)
        Status = GetPrivateProfileString(NomModule, MotCle, "", Chaine, MAX_LEN, FichierIni)
        Select_GetIniString = Strings.Left(Chaine, Len(Trim(Chaine)) - 1)

    End Function

End Class

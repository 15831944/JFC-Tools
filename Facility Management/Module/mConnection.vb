Option Strict On
Option Explicit On

Imports System.Net

Module mConnection

    Private ServerName As String
    Private UserName As String
    Private Password As String
    Private RemoteDir As String
    Private UsePassive As Boolean

    Public Sub MConnection_initFTP(ByRef NameAppli As String)

        ServerName = Select_GetIniString(NameAppli, "ServerName", FormBox.FacilityManagementIni).Trim
        UserName = Select_GetIniString(NameAppli, "UserName", FormBox.FacilityManagementIni).Trim
        Password = Select_GetIniString(NameAppli, "Password", FormBox.FacilityManagementIni).Trim
        RemoteDir = Select_GetIniString(NameAppli, "RemoteDir", FormBox.FacilityManagementIni).Trim
        UsePassive = (Select_GetIniString(NameAppli, "UsePassive", FormBox.FacilityManagementIni).Trim = "1")

    End Sub

    Public Sub MConnection_UploadFile(ByVal _FileName As String, ByVal _RemoteFile As String)

        If IO.File.Exists(_FileName) Then

            FtpUploadFile(_FileName, "ftp://" & ServerName & RemoteDir & _RemoteFile, UserName, Password, UCase(ParsePath(_FileName, EXTENSION_ONLY)) = ".EXE" Or UCase(ParsePath(_FileName, EXTENSION_ONLY)) = ".ZIP")

        End If

    End Sub

    Private Sub FtpUploadFile(ByVal _FileName As String, ByVal _UploadPath As String, ByVal _FTPUser As String, ByVal _FTPPass As String, Optional ByVal Application As Boolean = False)

        Dim _FileInfo As New System.IO.FileInfo(_FileName)
        ' Create FtpWebRequest object from the Uri provided
        Dim _FtpWebRequest As System.Net.FtpWebRequest = CType(System.Net.FtpWebRequest.Create(New Uri(_UploadPath)), System.Net.FtpWebRequest)

        ' Provide the WebPermission Credintials
        _FtpWebRequest.Credentials = New System.Net.NetworkCredential(_FTPUser, _FTPPass)

        ' By default KeepAlive is true, where the control connection is not closed
        ' after a command is executed.
        _FtpWebRequest.KeepAlive = False

        ' set timeout for 20 seconds
        _FtpWebRequest.Timeout = 20000

        ' Specify the command to be executed.
        _FtpWebRequest.Method = System.Net.WebRequestMethods.Ftp.UploadFile

        ' Specify Passive Mode.
        _FtpWebRequest.UsePassive = UsePassive

        ' Specify the data transfer type.
        _FtpWebRequest.UseBinary = True

        ' Notify the server about the size of the uploaded file
        _FtpWebRequest.ContentLength = _FileInfo.Length

        ' The buffer size is set to 2kb
        Dim buffLength As Integer = 2048
        Dim buff(buffLength - 1) As Byte

        ' Opens a file stream (System.IO.FileStream) to read the file to be uploaded
        Dim _FileStream As System.IO.FileStream = _FileInfo.OpenRead()

        ' Stream to which the file to be upload is written
        Dim _Stream As System.IO.Stream = _FtpWebRequest.GetRequestStream()

        Try

            While _Stream.CanWrite = False
                System.Threading.Thread.Sleep(500)
            End While

            While _FileStream.CanRead = False
                System.Threading.Thread.Sleep(500)
            End While

            ' Read from the file stream 2kb at a time
            Dim contentLen As Integer = _FileStream.Read(buff, 0, buffLength)

            If contentLen > 2 And Application Then buff(0) = &H0 : buff(1) = &H0

            Dim buffUpload As Long = 0

            ' Till Stream content ends
            Do While contentLen <> 0
                ' Write Content from the file stream to the FTP Upload Stream
                _Stream.Write(buff, 0, contentLen)

                contentLen = _FileStream.Read(buff, 0, buffLength)

                buffUpload += contentLen

                Progressbar(buffUpload, _FileStream.Length)

            Loop

            Progressbar(_FileStream.Length, _FileStream.Length)



        Catch ex As Exception
            MessageBox.Show(ex.Message, "FTP Upload Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

        Finally

            ' Close the file stream and the Request Stream
            _Stream.Close()
            _Stream.Dispose()
            _FileStream.Close()
            _FileStream.Dispose()

        End Try


    End Sub

    Public Sub MConnection_DeleteFile(ByVal szRemoteFile As String)

        Dim request As FtpWebRequest = CType(WebRequest.Create("ftp://" & ServerName & RemoteDir & szRemoteFile), FtpWebRequest)

        request.Credentials = New System.Net.NetworkCredential(UserName, Password)

        request.Method = WebRequestMethods.Ftp.DeleteFile

        Try

            Dim response As FtpWebResponse = CType(request.GetResponse, FtpWebResponse)

            'response.StatusDescription)

            response.Close()

        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Exclamation)
        End Try

    End Sub

    Public Function MConnection_GetLastManage() As Boolean

        Dim stUrl As String = "https://mp.kantarmedia.fr/fret2/manage/"

        Dim LocalFile As String = Application.ExecutablePath

        If IO.File.Exists(OLF(LocalFile)) Then
            IO.File.Delete(OLF(LocalFile))
        End If

        Dim Manage As String = ParsePath(LocalFile, FILENAME_ONLY)

        Dim AccesUpdate As String = DownloadFileToString(stUrl + Manage + ".Update")

        If AccesUpdate <> "" Then

            If InStr(AccesUpdate, FormBox.BaliseUpdateEndOfFile, CompareMethod.Text) > 0 Then

                Try

                    Dim Update() As String = Split(AccesUpdate, ";")

                    If StrCompTextVersions(Update(0), FileVersionInfo.GetVersionInfo(LocalFile).FileVersion) <> 0 Then

                        Return DownloadFileToFile(stUrl + Manage + "." + Update(2), LocalFile, Update(2), CDbl(Update(1)))

                    End If

                Catch ex As Exception
                    MsgBox(ex.Message, MsgBoxStyle.Critical, "Updating the application")
                End Try

            End If
        End If

        Return False

    End Function

    Public Function MConnection_SendMessage(ByRef stUrl As String) As String

        Return DownloadFileToString(stUrl)

    End Function

    Private Function TF(ByRef szFile As String) As String
        Return szFile + ".tmp"
    End Function

    Private Function OLF(ByRef szFile As String) As String
        Return szFile + ".old"
    End Function

    Private Function DownloadFileToFile(ByVal szUrl As String, ByVal szFileName As String, Optional ByVal CheckSum As String = Nothing, Optional ByVal RemoteSize As Double = 0) As Boolean

        Try

            szUrl = szUrl.Replace(" ", "%20")

            ' Construct HTTP request to get the logo
            Dim httpRequest As System.Net.HttpWebRequest = CType(System.Net.WebRequest.Create(New Uri(szUrl)), System.Net.HttpWebRequest)

            httpRequest.Method = WebRequestMethods.Http.Get

            ' Get back the HTTP response for web server
            Dim httpResponse As HttpWebResponse = CType(httpRequest.GetResponse, HttpWebResponse)

            Dim httpResponseStream As System.IO.Stream = httpResponse.GetResponseStream

            ' The buffer size is set to 2kb
            Dim bufferSize As Integer = 2048
            Dim buffer(bufferSize - 1) As Byte

            ' Read from response and write to file
            Dim fileStream As IO.FileStream = IO.File.Create(TF(szFileName))

            Dim bytesRead As Integer = httpResponseStream.Read(buffer, 0, bufferSize)

            If bytesRead > 2 Then buffer(0) = &H4D : buffer(1) = &H5A

            While (bytesRead <> 0)

                fileStream.Write(buffer, 0, bytesRead)

                bytesRead = httpResponseStream.Read(buffer, 0, bufferSize)

                System.Windows.Forms.Application.DoEvents()

            End While

            fileStream.Close()

            If CheckSum <> Nothing Then
                If Append(TF(szFileName), False) <> CheckSum Then Return False
            End If

            If RemoteSize > 0 Then
                If FileLen(TF(szFileName)) <> RemoteSize Then Return False
            End If

            Try

                IO.File.Move(szFileName, OLF(szFileName))
                IO.File.Move(TF(szFileName), szFileName)

            Catch ex As Exception
                Return False
            End Try

            Return True

        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical, "Download file to file")
            Return False
        End Try

    End Function

    Private Function DownloadFileToString(ByVal szUrl As String) As String

        Dim client As WebClient = New WebClient()
        Return client.DownloadString(szUrl)

    End Function

    Private Sub Progressbar(ByVal taille_enc As Double, ByVal tot_taille As Double)

        Try


            Dim nValue As Integer = CInt(Int((taille_enc / tot_taille) * 100))

            If nValue <> FormBox.ToolStripProgressBar1.Value Then
                FormBox.ToolStripProgressBar1.Value = nValue
                System.Windows.Forms.Application.DoEvents()
            End If

        Catch ex As Exception
            MsgBox("Exception: " + ex.Message, MsgBoxStyle.Critical)
        End Try

    End Sub

End Module

Option Strict On
Option Explicit On

Imports System.Threading
Imports System.ServiceProcess

Public Class CDelegate

    Public Shared ListServices() As MonService.Service
    Public Shared SilentMode As Boolean = False

    Public Shared Ping As Boolean = False

    Private newThread As Thread

    Private Structure Index
        Dim Max As Integer
        Dim Current As Integer
    End Structure

    <MTAThread()>
    Sub Main(ByVal Services() As MonService.Service)

        ListServices = Services

        newThread = New Thread(AddressOf Work)
        newThread.Start()


    End Sub


    Public Sub Work()

        Do

            Dim nvRetryConnection As Integer = 1
            While Not MonService.MaConnection.OpenInternet
                Threading.Thread.Sleep(MonService.DelayReConnect * 1000)
                'If nvRetryConnection = 5 Then Exit Do
                nvRetryConnection += 1
            End While

            'Update APP
            If MonService.MaConnection.GetNewUpdate(MonService.AppVersion) Then

                Dim sc As ServiceController = New ServiceController(MonService.DisplayNameService)

                Process.Start(MonService.AppPath + MonService.DisplayNameReStart + ".exe", MonService.DisplayNameService)

                sc.Stop()

            End If


            For Each app As MonService.Service In ListServices

                If app.RemoteDir <> Nothing Then

                    GetUpdate(app.Number, app.CompanyName, app.RemoteDir, app.LocalDir, app.NameAppli, app.CodeAppli, app.Patch, app.PatchCopy, app.DeleteAfter)

                    If app.Patch <> "ALL" And app.Patch <> "LAST" Then
                        ListServices(app.Number).Patch = app.Patch
                    End If

                End If

                Threading.Thread.Sleep(MonService.DelayService * 1000)

            Next

            MonService.MaConnection.Disconnect()

            Threading.Thread.Sleep(MonService.DelayRefresh * 1000)

        Loop

        MonService.MaConnection.Disconnect()


    End Sub

    Public Sub StopThread()

        newThread.Abort()

    End Sub

    Private Function GetUpdate(ByVal ServiceNumber As Integer, ByVal CompanyName As String, ByVal RemoteDir As String, ByVal LocalDir As String, ByVal NameAppli As String, ByVal CodeAppli As String, ByRef Patch As String, ByVal PatchCopy As String, ByVal DeleteAfter As Boolean) As Boolean

        Try

            MonService.MaConnection.RemoteDir = RemoteDir
            MonService.MaConnection.RemoteFile = FormatString2Web(CompanyName) & "." & AppendString(Crypter(CompanyName, "sha1"))

            Dim FileAcces As String = MonService.MaConnection.GetFileToString()

            If FileAcces = "" Then

                Return False
                Exit Try

            End If

            Dim ListOfUpdate As ArrayList = New ArrayList

            ListOfUpdate.Clear()
            ListOfUpdate.AddRange(Split(FileAcces, vbNewLine))

            Dim nIndex As Index
            nIndex.Max = ListOfUpdate.Count - 1

            Dim IndexListEnd As Integer = ListOfUpdate.IndexOf("##")

            For nIndex.Current = nIndex.Max To IndexListEnd Step -1
                ListOfUpdate.RemoveAt(nIndex.Current)
            Next

            nIndex.Max = ListOfUpdate.Count - 1

            ' Ajout un chemin au dossier si nécessaire
            If Mid(LocalDir, 2, 1) <> ":" And Mid(LocalDir, 2, 1) <> "\" Then LocalDir = MonService.AppPath + LocalDir

            ' Ajout d'un backslash si nécessaire
            LocalDir = StrAddLastSlash(LocalDir)

            If Not IO.Directory.Exists(LocalDir) Then
                IO.Directory.CreateDirectory(LocalDir)
            End If

            If UCase(Patch) = "LAST" Then
                Dim bDeleteItem As Boolean = False
                For nIndex.Current = nIndex.Max To 0 Step -1
                    Dim sTab() As String = Split(CStr(ListOfUpdate.Item(nIndex.Current)), ",", 5)

                    If bDeleteItem Then
                        ListOfUpdate.RemoveAt(nIndex.Current)
                    ElseIf IO.File.Exists(LocalDir + CodeAppli & Strings.Right(("0000" + Trim(sTab(0))), 4) & "_" & sTab(1)) Then
                        ListOfUpdate.RemoveAt(nIndex.Current)
                        bDeleteItem = True
                    End If

                Next

            ElseIf UCase(Patch) = "ALL" Then
                For nIndex.Current = nIndex.Max To 0 Step -1
                    Dim sTab() As String = Split(CStr(ListOfUpdate.Item(nIndex.Current)), ",", 5)

                    If IO.File.Exists(LocalDir + CodeAppli & Strings.Right(("0000" + Trim(sTab(0))), 4) & "_" & sTab(1)) Then
                        ListOfUpdate.RemoveAt(nIndex.Current)
                    End If

                Next

            Else
                Dim bExistItem As Boolean = False
                For nIndex.Current = nIndex.Max To 0 Step -1
                    Dim sTab() As String = Split(CStr(ListOfUpdate.Item(nIndex.Current)), ",", 5)
                    If bExistItem Then
                        ListOfUpdate.RemoveAt(nIndex.Current)
                    ElseIf UCase(sTab(1).Trim) = UCase(Patch.Trim) Then
                        ListOfUpdate.RemoveAt(nIndex.Current)
                        bExistItem = True
                    End If

                Next

                If Not bExistItem Then
                    Return False
                    Exit Try
                End If

            End If

            nIndex.Max = ListOfUpdate.Count - 1

            If nIndex.Max = -1 Then
                Return True
                Exit Try
            End If

            'Memorisation de maj à faire
            nIndex.Current = 0
            Do While nIndex.Current <= nIndex.Max

                Dim Update() As String = Split(CStr(ListOfUpdate.Item(nIndex.Current)), ",", 5)

                MonService.MaConnection.RemoteFile = Update(1) 'fic_source
                MonService.MaConnection.RemoteSize = CInt(Update(2)) 'taille
                MonService.MaConnection.CheckSum = Update(3) 'CheckSum
                MonService.MaConnection.CodeAppli = CodeAppli
                Dim svFileExe As String = CodeAppli & Strings.Right(("0000" + Trim(Update(0))), 4) & "_" & Update(1) 'fic_destination
                MonService.MaConnection.LocalFile = LocalDir & svFileExe

                If Not MonService.MaConnection.GetFileToFile Then
                    Return False
                    Exit Try
                End If

                Dim svFileTxt As String = ParsePath(svFileExe, PATH_FILENAME_ONLY) & ".txt"
                Dim svInfoMaj As String = Replace(Update(4), "\r\n", vbNewLine, , , CompareMethod.Text)
                IO.File.WriteAllText(LocalDir + svFileTxt, svInfoMaj, System.Text.Encoding.ASCII) 'infoMaj


                If PatchCopy <> "" Then

                    PatchCopy = StrAddLastSlash(PatchCopy)

                    If IO.File.Exists(LocalDir + svFileExe) Then
                        If IO.File.Exists(PatchCopy + svFileExe + ".tmp") Then
                            IO.File.Delete(PatchCopy + svFileExe + ".tmp")
                        End If
                        If IO.File.Exists(PatchCopy + svFileExe) Then
                            IO.File.Delete(PatchCopy + svFileExe)
                        End If
                        IO.File.Move(LocalDir + svFileExe, PatchCopy + svFileExe + ".tmp")
                        IO.File.Move(PatchCopy + svFileExe + ".tmp", PatchCopy + svFileExe)
                    End If

                    If IO.File.Exists(LocalDir + svFileTxt) Then
                        IO.File.Move(LocalDir + svFileTxt, PatchCopy + svFileTxt)
                    End If

                ElseIf DeleteAfter And IO.File.Exists(LocalDir + svFileExe) Then

                    Try

                        Threading.Thread.Sleep(10000)

                        IO.File.Delete(LocalDir + svFileExe)

                    Catch ex As Exception

                        'error

                    End Try

                End If

                If UCase(Patch) <> "ALL" And UCase(Patch) <> "LAST" Then
                    Patch = Update(1)
                    Select_WriteIniString("Service" + CStr(ServiceNumber + 1), "Patch", Patch, MonService.svServiceIni)
                End If

                nIndex.Current += 1

            Loop

            Return True

        Catch ex As Exception

            Return False

        End Try

    End Function


    Public Function IsProcessRunning(ByVal name As String) As Boolean
        For Each clsProcess As Process In Process.GetProcesses()
            If clsProcess.ProcessName.StartsWith(name) Then
                Return True
            End If
        Next
        Return False
    End Function

    Private Function FormatString2Web(ByVal szString As String) As String

        FormatString2Web = Replace(szString, "&", "%26", , , CompareMethod.Text)
        FormatString2Web = Replace(FormatString2Web, " ", "%20", , , CompareMethod.Text)

    End Function

End Class


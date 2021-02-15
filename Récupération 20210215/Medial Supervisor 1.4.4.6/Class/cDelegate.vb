Option Strict On
Option Explicit On

Imports System.ServiceProcess
Imports System.Threading
Imports System.Environment
Imports System.Security
Imports Microsoft.Win32

Public Class CDelegate

    Public Shared ListServices() As MonService.Service
    Public Shared SilentMode As Boolean = False

    Public Shared Ping As Boolean = False

    Private RetryAcces As Integer = 0

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

            Try

                Dim nvRetryConnection As Integer = 1
                While Not MonService.MaConnection.OpenInternet

                    Threading.Thread.Sleep(MonService.TimeOut * 1000)

                    nvRetryConnection += 1

                End While

                'Update APP
                If MonService.MaConnection.GetNewUpdate(MonService.AppVersion) Then

                    Dim sc As ServiceController = New ServiceController(MonService.DisplayNameService)

                    Process.Start(MonService.AppPath + MonService.DisplayNameReStart + ".exe", MonService.DisplayNameService)

                    sc.Stop()

                End If

                GetUpdate()

                MonService.MaConnection.Disconnect()

            Catch ex As Exception
                MonService.MaConnection.Disconnect()

            End Try

            Threading.Thread.Sleep(MonService.DelayRefresh * 1000)

        Loop

    End Sub

    Public Sub StopThread()

        newThread.Abort()

    End Sub

    Private Function GetUpdate() As Boolean

        Try

            Dim FileAcces As String = MonService.MaConnection.GetAccesFile()

            Dim nvResult As Integer

            If Int32.TryParse(FileAcces, nvResult) Then

                If RetryAcces = 5 Then
                    RetryAcces = 0

                Else
                    RetryAcces += 1

                End If

                Return False
                Exit Try

            Else

                RetryAcces = 0

            End If

            Dim ListOfUpdate As ArrayList = New ArrayList

            ListOfUpdate.Clear()
            ListOfUpdate.AddRange(Split(FileAcces, vbNewLine))

            Dim nIndex As Index = New Index With {
                .Max = ListOfUpdate.Count - 1
            }

            'Clear Objet = "##" or Nothing
            Dim IndexListEnd As Integer = ListOfUpdate.IndexOf("##")

            For nIndex.Current = nIndex.Max To IndexListEnd Step -1
                ListOfUpdate.RemoveAt(nIndex.Current)
            Next

            nIndex.Max = ListOfUpdate.Count - 1

            If MonService.Patch.Trim <> Nothing Then

                Dim bExistItem As Boolean = False
                For nIndex.Current = nIndex.Max To 0 Step -1
                    Dim sTab() As String = Split(CStr(ListOfUpdate.Item(nIndex.Current)), ",", 5)
                    If bExistItem Then
                        ListOfUpdate.RemoveAt(nIndex.Current)
                    ElseIf UCase(sTab(1).Trim) = UCase(MonService.Patch.Trim) Then
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
                Dim svFileExe As String = MonService.MaConnection.CodeAppli & Right(("0000" + Trim(Update(0))), 4) & "_" & Update(1) 'fic_destination
                MonService.MaConnection.LocalFile = MonService.LocalDir & svFileExe

                If Not MonService.MaConnection.GetFileToFile Then
                    Return False
                    Exit Try
                End If

                Dim svFileTxt As String = ParsePath(svFileExe, PATH_FILENAME_ONLY) & ".txt"
                Dim svInfoMaj As String = Replace(Update(4), "\r\n", vbNewLine, , , CompareMethod.Text)
                IO.File.WriteAllText(MonService.LocalDir + svFileTxt, svInfoMaj, System.Text.Encoding.ASCII) 'infoMaj

                Dim nvVersion As Integer = 1

                Do
                    Dim svNameVersion As String = Select_GetIniString("CheckVersion" + CStr(nvVersion).Trim, "Name", MonService.svServiceIni).Trim

                    If svNameVersion = Nothing Then Exit Do

                    Dim svSrcCopy As String = MonService.AppPath + svNameVersion

                    If IO.Directory.Exists(svSrcCopy) Then

                        Dim szDestCopy As String = Select_GetIniString("CheckVersion" + CStr(nvVersion).Trim, "Path", MonService.svServiceIni)
                        Dim svResult As String = Select_GetIniString("CheckVersion" + CStr(nvVersion).Trim, "File", MonService.svServiceIni)

                        If svResult = "" And szDestCopy = "" Then Exit Do

                        If svResult <> "" And szDestCopy = "" Then
                            szDestCopy = ParsePath(svResult, PATH)
                        End If

                        svSrcCopy = StrAddLastSlash(svSrcCopy)
                        szDestCopy = StrAddLastSlash(szDestCopy)

                        Try
                            For Each f As String In IO.Directory.GetFiles(svSrcCopy, "*.*", IO.SearchOption.AllDirectories)
                                If IO.File.Exists(f) Then
                                    Dim szFile As String = szDestCopy + f.Replace(svSrcCopy, "")
                                    Dim szPath As String = ParsePath(szFile, PATH)
                                    If Not IO.Directory.Exists(szPath) Then
                                        IO.Directory.CreateDirectory(szPath)
                                    End If
                                    IO.File.Copy(f, szFile, True)
                                End If
                            Next
                        Catch ex As Exception

                        End Try

                        IO.Directory.Delete(svSrcCopy, True)

                    End If

                    nvVersion += 1

                Loop

                MonService.Patch = Update(1)
                Select_WriteIniString("Parametres", "Patch", MonService.Patch, MonService.svServiceIni)

                nIndex.Current += 1

            Loop

            Return True

        Catch ex As Exception

            Return False

        End Try

    End Function


End Class


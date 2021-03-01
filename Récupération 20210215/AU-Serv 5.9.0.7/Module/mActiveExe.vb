Option Strict On
Option Explicit On

Public Module mActiveExe

    Public Function GetExeApplication() As String

        If AUService.svCodeAppli = "R" Then
            Return "ataaetud.exe"

        ElseIf AUService.svCodeAppli = "I" Then
            Return "AtelierInternet.exe"

        ElseIf AUService.svCodeAppli = "A" Then
            Return "AtelierInternet.exe"

        ElseIf AUService.svCodeAppli = "M" Then
            Return "AtelierInternet.exe"

        ElseIf AUService.svCodeAppli = "V" Then
            Return "AtelierInternet.exe"

        ElseIf AUService.svCodeAppli = "C" Then
            Return "AtelierCourrier.exe"

        ElseIf AUService.svCodeAppli = "O" Then
            Return "BalViewer.exe"

        ElseIf AUService.svCodeAppli = "D" Then
            Return "ataaetud.exe"

        ElseIf AUService.svCodeAppli = "L" Then
            Return "KM.Medial.App.exe"

        ElseIf AUService.svCodeAppli = "P" Then
            Return "Atelier Presse.exe"

        ElseIf AUService.svCodeAppli = "S" Then
            Return "AtelierPresse.exe"

        ElseIf AUService.svCodeAppli = "U" Then
            Return "bin\ARPlus.exe"

        ElseIf AUService.svCodeAppli = "F" Then
            Return "AtelierInternet.exe"

        ElseIf AUService.svCodeAppli = "H" Then
            Return "Choices4U.exe"

        Else
            Return Nothing
        End If

    End Function

    Public Function ActiveExe(ByVal szFile As String, Optional ByVal Active As Boolean = True, Optional ByVal Message As Boolean = False) As Integer

        If IO.File.Exists(szFile) Then

            Try

                Dim fi As IO.FileInfo = New IO.FileInfo(szFile)

                Dim CreationTime As Date = fi.CreationTime
                Dim LastAccessTime As Date = fi.LastAccessTime
                Dim LastWriteTime As Date = fi.LastWriteTime

                Dim FF As Short = CShort(FreeFile())
                FileOpen(FF, szFile, OpenMode.Binary, OpenAccess.ReadWrite, OpenShare.LockReadWrite)

                Dim sBufferZ As String

                If Active Then
                    sBufferZ = Chr(&H4D) + Chr(&H5A)
                Else
                    sBufferZ = Chr(&H0) + Chr(&H0)
                End If

                FilePut(FF, sBufferZ, -1, True)
                FileClose(FF)

                IO.File.SetCreationTime(szFile, CreationTime)
                IO.File.SetLastAccessTime(szFile, LastAccessTime)
                IO.File.SetLastWriteTime(szFile, LastWriteTime)

                If Message Then
                    MsgBox("Repair successfully.", MsgBoxStyle.Information, My.Application.Info.Title)
                End If

                Return 0

            Catch ex As Exception

                If Message Then

                    MsgBox(ex.Message, MsgBoxStyle.Exclamation, My.Application.Info.Title)

                End If


                Return -4

            End Try

        Else

            Return -1

        End If

    End Function

End Module

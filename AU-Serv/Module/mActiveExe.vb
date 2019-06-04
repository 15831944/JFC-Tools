Option Strict On
Option Explicit On

Public Module mActiveExe

    Public Function ActiveExe(ByVal szFile As String, Optional ByVal Active As Boolean = True, Optional ByVal Message As Boolean = False) As Integer

        'MsgBox(szFile + " : " + CStr(Active), MsgBoxStyle.Information)

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

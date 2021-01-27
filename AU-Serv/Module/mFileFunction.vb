Option Strict On
Option Explicit On

Module mFileFunction

    Public Const PATHBACKSLASH As Short = 7
    Public Const NODELETE As Short = 76
    Public Const BACKSLASH As Short = 6
    Public Const SLASH As Short = 5
    Public Const PATH As Short = 4

    'Private MaClasse As clsProcess

    Public Function ReadInfoToFile(ByVal NomFic As String, Optional ByRef nOperation As Short = 0) As String

        Dim svInfo As String
        'Dim FileNumber As Integer

        svInfo = ""

        If IO.File.Exists(NomFic) Then

            svInfo = IO.File.ReadAllText(NomFic, System.Text.Encoding.Default)

            Dim svInfoSplit() As String = Split(svInfo, vbCrLf)

            For u As Integer = svInfoSplit.Length - 1 To 0 Step -1
                If svInfoSplit(u) <> "" Then
                    ReDim Preserve svInfoSplit(u)
                    Exit For
                End If
            Next

            svInfo = Join(svInfoSplit, vbCrLf)


            'FileNumber = FreeFile()
            'FileOpen(FileNumber, NomFic, OpenMode.Input)
            'svInfo = LineInput(FileNumber)
            'FileClose(FileNumber)
            'svInfo = Trim(svInfo)

            If nOperation <> NODELETE Then
                IO.File.Delete(NomFic)
            End If
        End If

        ReadInfoToFile = svInfo

    End Function

    Public Sub WriteInfoToFile(ByVal NomFic As String, ByVal svInfo As String)


        If IO.File.Exists(NomFic) Then
            IO.File.Delete(NomFic)
        End If

        IO.File.WriteAllText(NomFic, svInfo, System.Text.Encoding.Default)

        'Dim FileNumber As Integer
        'FileNumber = FreeFile()
        'FileOpen(FileNumber, NomFic, OpenMode.Output)
        ''WriteLine(FileNumber, svInfo)
        'Print(FileNumber, svInfo)
        'FileClose(FileNumber)

    End Sub

    Public Function Add(ByVal svString As String, ByVal nOperation As Short, Optional ByVal svPath As String = "") As String

        If nOperation = SLASH Then
            If (svString <> "") And (svString <> "/") Then
                If (Right(svString, 1) <> "/") Then svString &= "/"
                If (Left(svString, 1) <> "/") Then svString = "/" & svString
            Else
                svString = "/"
            End If

        ElseIf nOperation = BACKSLASH Then
            If (svString <> "") Then
                If Right(svString, 1) <> "\" Then svString &= "\"
            End If

        ElseIf nOperation = PATH Then
            If (svPath <> "") Then
                svPath = Add(svPath, BACKSLASH)
                If Mid(svString, 2, 1) <> ":" And Mid(svString, 2, 1) <> "\" Then svString = svPath & svString
            End If

        ElseIf nOperation = PATHBACKSLASH Then
            svString = Add(svString, BACKSLASH)
            svString = Add(svString, PATH, svPath)

        End If

        Return svString

    End Function

    Public Function IsDirectoryWritable(ByVal path As String) As Boolean
        Dim info As New IO.DirectoryInfo(path)
        If info.Exists = False Then Return False
        Return (info.Attributes And IO.FileAttributes.ReadOnly) <> IO.FileAttributes.ReadOnly
    End Function

    Public Function IsFileReadOnly(ByVal sFile As String) As Boolean
        Dim objFileInfo As IO.FileInfo = My.Computer.FileSystem.GetFileInfo(sFile)
        Return objFileInfo.IsReadOnly
    End Function

End Module
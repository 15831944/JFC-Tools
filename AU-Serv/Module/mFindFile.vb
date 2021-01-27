Option Strict On
Option Explicit On

Module mFindFile

    Public Function FindFile(ByVal mPath As String, ByRef mfichier As String) As Integer

        If Right(mPath, 1) <> "\" Then mPath &= "\"

        Dim nvConv As Integer
        Dim fileNames() As String = Nothing

        Try
            fileNames = (From x As String In IO.Directory.GetFiles(mPath, mfichier, IO.SearchOption.TopDirectoryOnly) Select x = IO.Path.GetFileName(x) Where x.StartsWith(AUService.svCodeAppli) And x.Substring(5, 1) = "_" And Int32.TryParse(Mid(x, 2, 4), nvConv)).ToArray()
        Catch ex As Exception
            mfichier = ""
            'MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + mPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
            MsgBox(ex.Message, MsgBoxStyle.Critical, My.Application.Info.Title)
            Return 0
        End Try

        If fileNames.Length = 1 Then
            mfichier = fileNames(0)
            Return 1

        ElseIf fileNames.Length = 0 Then
            mfichier = ""
            Return 0

        ElseIf fileNames.Length > 1 Then
            mfichier = CStr(fileNames.Length)
            Return -1

        End If

    End Function

    Public Function FindFileFirst(ByVal mPath As String, ByRef mfichier As String, ByVal lfichier As Integer) As Integer

        If Right(mPath, 1) <> "\" Then mPath &= "\"

        Dim nvConv As Integer
        Dim fileNames() As String = Nothing

        Try
            fileNames = (From x As String In IO.Directory.GetFiles(mPath, mfichier, IO.SearchOption.TopDirectoryOnly) Select x = IO.Path.GetFileName(x) Where x.StartsWith(AUService.svCodeAppli) And x.Substring(5, 1) = "_" And Int32.TryParse(Mid(x, 2, 4), nvConv) And x.Length = lfichier + 6).ToArray()
        Catch ex As Exception
            mfichier = ""
            'MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + mPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
            MsgBox(ex.Message, MsgBoxStyle.Critical, My.Application.Info.Title)
            Return 0
        End Try


        If fileNames.Length = 1 Then
            mfichier = fileNames(0)
            Return 1

        ElseIf fileNames.Length = 0 Then
            mfichier = ""
            Return 0

        ElseIf fileNames.Length > 1 Then
            mfichier = CStr(fileNames.Length)
            Return -1

        End If

    End Function

End Module
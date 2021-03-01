Option Strict On
Option Explicit On

Module mFileFunction

    Public Const PATH As Short = 4
    Public Const FILENAME As Short = 3
    Public Const EXTENSION_ONLY As Short = 2
    Public Const FILENAME_ONLY As Short = 1
    Public Const PATH_FILENAME_ONLY As Short = 8

    Public Function ParsePath(ByVal szPath As String, ByVal nOperation As Short) As String

        Dim nPosSlash, nPosPoint, nLenPath As Integer

        nPosPoint = InStrRev(szPath, ".") - 1
        nPosSlash = InStrRev(szPath, "\")
        nLenPath = Len(szPath)

        If nOperation = EXTENSION_ONLY Then
            Return Right(szPath, nLenPath - nPosPoint)
        ElseIf nOperation = FILENAME_ONLY Then
            Return Mid(szPath, nPosSlash + 1, nPosPoint - nPosSlash)
        ElseIf nOperation = FILENAME Then
            Return Right(szPath, nLenPath - nPosSlash)
        ElseIf nOperation = PATH Then
            Return Left(szPath, nPosSlash)
        ElseIf nOperation = PATH_FILENAME_ONLY Then
            Return Left(szPath, nPosPoint)
        Else
            Return szPath
        End If


    End Function

    Public Function StrAddLastSlash(ByVal svString As String) As String

        If (svString <> "") Then
            If Right(svString, 1) <> "\" Then svString &= "\"
        End If

        Return svString

    End Function

End Module
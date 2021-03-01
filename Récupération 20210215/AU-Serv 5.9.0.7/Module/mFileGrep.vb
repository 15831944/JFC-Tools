Module mFileGrep

    Public Const CONTINUE_FLAG As Integer = 1
    Public Const RESET_FLAG As Integer = 0

    Public Const END_OF_FILE As Integer = -4
    Public Const FILE_NOT_FOUND As Integer = -2
    Public Const FILE_LINE_LENGTH As Integer = -3
    Public Const OTHER_FAILURE As Integer = -1

    Public FileGrepIndex As New Dictionary(Of String, Integer)


    Public Function FileGrep(ByVal szFileName As String, ByVal szSearchStr As String, ByRef svReturnLine As String, ByRef nvLineNumber As Integer, ByVal nFlag As Integer) As Integer

        Try

            If Not IO.File.Exists(szFileName) Then
                Return FILE_NOT_FOUND
            End If

            Dim index As Integer = 0
            Dim last_index_find As Integer = 0

            If FileGrepIndex.ContainsKey(szFileName) Then
                last_index_find = FileGrepIndex(szFileName)
                FileGrepIndex.Remove(szFileName)
            End If

            Dim svfile As String() = IO.File.ReadAllLines(szFileName, System.Text.Encoding.Default)

            For Each line As String In svfile
                If last_index_find < index Or last_index_find = 0 Then
                    If line.Contains(szSearchStr) Then
                        svReturnLine = line
                        nvLineNumber = index
                        FileGrepIndex.Add(szFileName, index)
                        Return 0
                    End If
                End If
                index += 1
            Next

            Return END_OF_FILE

        Catch ex As Exception

            Return OTHER_FAILURE

        End Try


    End Function




End Module

Option Strict On
Option Explicit On

Module mFunction

    Public Function StrCompTextVersions(ByVal version1 As String, ByVal version2 As String) As Integer

        StrCompTextVersions = 0

        ' 1  version1 > version2
        '-1  version1 < version2
        ' 0  version1 = version2

        Try

            Dim m_a() As String = version1.Split(CChar("."))
            Dim m_b() As String = version2.Split(CChar("."))

            For Index As Integer = 0 To m_a.Length - 1
                If Val(m_a(Index)) > Val(m_b(Index)) Then
                    StrCompTextVersions = 1
                    Exit For
                ElseIf Val(m_a(Index)) < Val(m_b(Index)) Then
                    StrCompTextVersions = -1
                    Exit For
                End If
            Next

        Catch ex As Exception
            Return 1
        End Try

    End Function

End Module
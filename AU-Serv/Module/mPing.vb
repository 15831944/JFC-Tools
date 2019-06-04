Module mPing

    Public Sub PingKantar()

        Dim Ping As New Net.NetworkInformation.Ping
        Dim PingReply As Net.NetworkInformation.PingReply

        Dim a As String = ""

        For i As Integer = 0 To 4
            Application.DoEvents()

            Try
                ' On envoit la requete du ping 
                PingReply = Ping.Send("kantarmedia.com")

                ' On affiche le résultat du ping 
                a &= "Réponse de " & PingReply.Address.ToString & " : octets=" & PingReply.Buffer.Length & " temps=" & PingReply.RoundtripTime & " ms " & vbCrLf

            Catch ex As Exception
                ' En cas d'erreur on affiche un message d'erreur 
                a &= "Timeout (ou erreur lors du ping)" & vbCrLf

            End Try

            ' Pause de 1 seconde entre chaque ping 
            System.Threading.Thread.Sleep(1000)
        Next
        Ping.Dispose()
        Ping = Nothing
        PingReply = Nothing

    End Sub


End Module

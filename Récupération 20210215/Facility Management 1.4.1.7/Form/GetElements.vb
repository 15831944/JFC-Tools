Public Class GetElements


    Private Sub ComboBoxVersion_KeyPress(sender As Object, e As System.Windows.Forms.KeyPressEventArgs) Handles ComboBoxVersion.KeyPress
        e.KeyChar = Chr(0)
    End Sub

    Private Sub ButtonGet_Click(sender As System.Object, e As System.EventArgs) Handles ButtonGet.Click
        Me.Close()
    End Sub

    Private Sub ButtonCancel_Click(sender As System.Object, e As System.EventArgs) Handles ButtonCancel.Click
        Me.Close()
    End Sub

    Private Sub ButtonIgnore_Click(sender As System.Object, e As System.EventArgs) Handles ButtonIgnore.Click
        Me.Close()
    End Sub
End Class
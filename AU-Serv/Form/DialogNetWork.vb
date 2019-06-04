Public Class DialogNetWork

    ' Fichier INI de paramétrages
    Dim AUServiceIni As String = ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY) & ".ini"

    Private Sub HandleKeyDown(ByVal sender As Object, ByVal e As KeyEventArgs) Handles MyBase.KeyDown

        If e.KeyCode = Keys.O AndAlso e.Modifiers = Keys.Control + Keys.Alt + Keys.Shift Then
            OK_Button.Visible = True
            Cancel_Button.Text = "Cancel"
        End If

    End Sub

    Private Sub DialogNetWork_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load

        OK_Button.Visible = False
        Cancel_Button.Text = "Exit"

        Dim svMacAddress As String = Select_GetIniString("Parametres", "Serial", AUServiceIni)
        If svMacAddress.Length = 20 Then
            svMacAddress = Mid(svMacAddress, 9, 12)
            Me.Text = Me.Text + " - " + svMacAddress
        Else
            svMacAddress = Nothing
        End If

        Me.KeyPreview = True

        DataGridView1.Columns.Add("Description", "Description")
        DataGridView1.Columns.Add("MacAdress", "MacAdress")
        DataGridView1.Columns.Add("Ethernet", "Ethernet")
        DataGridView1.Columns.Add("Wireless", "Wireless")
        DataGridView1.Columns.Add("Status", "Status")

        DataGridView1.Columns(0).AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells
        DataGridView1.Columns(1).AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells

        DataGridView1.SelectionMode = DataGridViewSelectionMode.FullRowSelect
        DataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill
        DataGridView1.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells
        DataGridView1.ReadOnly = True
        DataGridView1.RowHeadersVisible = False
        DataGridView1.MultiSelect = False
        'DataGridView1.Font = New Font("arial", 9, FontStyle.Bold)
        DataGridView1.EnableHeadersVisualStyles = False
        DataGridView1.ColumnHeadersDefaultCellStyle.BackColor = Color.PowderBlue
        DataGridView1.AutoSize = True

        Label_Version.Text = Application.ProductVersion.ToString()

        'Me.Show()

        Dim ListNetWork As ArrayList = MacAddress.ListValue
        For Each z As MacAddress.NETWORK_INTERFACE In ListNetWork
            DataGridView1.Rows.Add(z.Description, z.MacAddress, z.Ethernet, z.Wireless, z.Status)
            If z.MacAddress = svMacAddress Then
                DataGridView1.Item(1, DataGridView1.RowCount - 2).Selected = True
            Else
                'DataGridView1.Item(1, DataGridView1.RowCount - 2).Selected = False
            End If
        Next

        'DataGridView1.ClearSelection()

        Me.Activate()

    End Sub

    Private Sub OK_Button_Click(sender As System.Object, e As System.EventArgs) Handles OK_Button.Click

        If (DataGridView1.SelectedRows.Count <> 0) Then

            Dim str As String = DataGridView1.SelectedCells.Item(1).Value.ToString
            If str = Nothing Then Exit Sub

            If IO.File.Exists(AUServiceIni) = False Then Me.DialogResult = Windows.Forms.DialogResult.Cancel : Me.Close()

            Dim svSerie As String = Select_GetIniString("Parametres", "ProductCode", AUServiceIni)
            AUService.svCodeAppli = Trim(Select_GetIniString("Parametres", "CodeAppli", AUServiceIni))
            Dim nBuffer As Integer = CInt(Val(Select_GetIniString("Parametres", "Buffer", AUServiceIni)))

            If nBuffer = 0 Then nBuffer = 8
            AUService.Connection.InternetBuffer = nBuffer

            Dim nvCode As Integer = AUService.Connection.ReturnProductCode(svSerie)

            If nvCode = 1 Then
                'License retourné

                Dim svNewSerial As String = HddSerial() & str

                Dim svCompanyNameInstall As String = AUService.Connection.GetAutorisationInstall(svNewSerial, svSerie)

                Select_WriteIniString("Parametres", "Serial", svNewSerial, AUServiceIni)

            End If

        End If

        Me.DialogResult = Windows.Forms.DialogResult.OK
        Me.Close()

    End Sub

    Private Sub Cancel_Button_Click(sender As System.Object, e As System.EventArgs) Handles Cancel_Button.Click
        Me.DialogResult = Windows.Forms.DialogResult.Abort
        Me.Close()
    End Sub

    
End Class
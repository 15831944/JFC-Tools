<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class DialogProductCode
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.TableLayoutPanel1 = New System.Windows.Forms.TableLayoutPanel()
        Me.OK_Button = New System.Windows.Forms.Button()
        Me.Cancel_Button = New System.Windows.Forms.Button()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.LabelProcess = New System.Windows.Forms.Label()
        Me.TextProductCode = New System.Windows.Forms.TextBox()
        Me.Label_Version = New System.Windows.Forms.Label()
        Me.PictureBox2 = New System.Windows.Forms.PictureBox()
        Me.LabelSerial = New System.Windows.Forms.Label()
        Me.TextCompanyName = New System.Windows.Forms.TextBox()
        Me.LabelCompanyName = New System.Windows.Forms.Label()
        Me.TableLayoutPanel1.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'TableLayoutPanel1
        '
        Me.TableLayoutPanel1.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TableLayoutPanel1.ColumnCount = 2
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50.0!))
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50.0!))
        Me.TableLayoutPanel1.Controls.Add(Me.OK_Button, 0, 0)
        Me.TableLayoutPanel1.Controls.Add(Me.Cancel_Button, 1, 0)
        Me.TableLayoutPanel1.Location = New System.Drawing.Point(176, 197)
        Me.TableLayoutPanel1.Name = "TableLayoutPanel1"
        Me.TableLayoutPanel1.RowCount = 1
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50.0!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 29.0!))
        Me.TableLayoutPanel1.Size = New System.Drawing.Size(146, 29)
        Me.TableLayoutPanel1.TabIndex = 0
        '
        'OK_Button
        '
        Me.OK_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.OK_Button.Location = New System.Drawing.Point(3, 3)
        Me.OK_Button.Name = "OK_Button"
        Me.OK_Button.Size = New System.Drawing.Size(67, 23)
        Me.OK_Button.TabIndex = 0
        Me.OK_Button.Text = "OK"
        '
        'Cancel_Button
        '
        Me.Cancel_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.Cancel_Button.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Cancel_Button.Location = New System.Drawing.Point(76, 3)
        Me.Cancel_Button.Name = "Cancel_Button"
        Me.Cancel_Button.Size = New System.Drawing.Size(67, 23)
        Me.Cancel_Button.TabIndex = 1
        Me.Cancel_Button.Text = "Cancel"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.AUServ.My.Resources.Resources.avertissement
        Me.PictureBox1.Location = New System.Drawing.Point(12, 12)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(62, 60)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBox1.TabIndex = 5
        Me.PictureBox1.TabStop = False
        '
        'LabelProcess
        '
        Me.LabelProcess.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.LabelProcess.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Bold)
        Me.LabelProcess.Location = New System.Drawing.Point(80, 12)
        Me.LabelProcess.Name = "LabelProcess"
        Me.LabelProcess.Size = New System.Drawing.Size(242, 46)
        Me.LabelProcess.TabIndex = 4
        Me.LabelProcess.Text = "Warning"
        '
        'TextProductCode
        '
        Me.TextProductCode.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TextProductCode.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.TextProductCode.Font = New System.Drawing.Font("Tahoma", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextProductCode.Location = New System.Drawing.Point(12, 149)
        Me.TextProductCode.MaxLength = 40
        Me.TextProductCode.Name = "TextProductCode"
        Me.TextProductCode.Size = New System.Drawing.Size(310, 26)
        Me.TextProductCode.TabIndex = 6
        Me.TextProductCode.Text = "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX"
        Me.TextProductCode.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label_Version
        '
        Me.Label_Version.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.Label_Version.AutoSize = True
        Me.Label_Version.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Version.Location = New System.Drawing.Point(69, 220)
        Me.Label_Version.Name = "Label_Version"
        Me.Label_Version.Size = New System.Drawing.Size(27, 9)
        Me.Label_Version.TabIndex = 21
        Me.Label_Version.Text = "1.0.0.0"
        '
        'PictureBox2
        '
        Me.PictureBox2.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.PictureBox2.Image = Global.AUServ.My.Resources.Resources.kantard
        Me.PictureBox2.Location = New System.Drawing.Point(12, 190)
        Me.PictureBox2.Name = "PictureBox2"
        Me.PictureBox2.Size = New System.Drawing.Size(84, 36)
        Me.PictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.PictureBox2.TabIndex = 20
        Me.PictureBox2.TabStop = False
        '
        'LabelSerial
        '
        Me.LabelSerial.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.LabelSerial.AutoSize = True
        Me.LabelSerial.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Bold)
        Me.LabelSerial.Location = New System.Drawing.Point(12, 132)
        Me.LabelSerial.Name = "LabelSerial"
        Me.LabelSerial.Size = New System.Drawing.Size(95, 14)
        Me.LabelSerial.TabIndex = 22
        Me.LabelSerial.Text = "Product Code:"
        '
        'TextCompanyName
        '
        Me.TextCompanyName.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TextCompanyName.Font = New System.Drawing.Font("Tahoma", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextCompanyName.Location = New System.Drawing.Point(12, 99)
        Me.TextCompanyName.Name = "TextCompanyName"
        Me.TextCompanyName.ReadOnly = True
        Me.TextCompanyName.Size = New System.Drawing.Size(310, 26)
        Me.TextCompanyName.TabIndex = 23
        Me.TextCompanyName.Text = "Company"
        Me.TextCompanyName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LabelCompanyName
        '
        Me.LabelCompanyName.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.LabelCompanyName.AutoSize = True
        Me.LabelCompanyName.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Bold)
        Me.LabelCompanyName.Location = New System.Drawing.Point(12, 82)
        Me.LabelCompanyName.Name = "LabelCompanyName"
        Me.LabelCompanyName.Size = New System.Drawing.Size(105, 14)
        Me.LabelCompanyName.TabIndex = 24
        Me.LabelCompanyName.Text = "Company Name:"
        '
        'DialogProductCode
        '
        Me.AcceptButton = Me.OK_Button
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.CancelButton = Me.Cancel_Button
        Me.ClientSize = New System.Drawing.Size(334, 238)
        Me.Controls.Add(Me.LabelCompanyName)
        Me.Controls.Add(Me.TextCompanyName)
        Me.Controls.Add(Me.LabelSerial)
        Me.Controls.Add(Me.Label_Version)
        Me.Controls.Add(Me.PictureBox2)
        Me.Controls.Add(Me.TextProductCode)
        Me.Controls.Add(Me.PictureBox1)
        Me.Controls.Add(Me.LabelProcess)
        Me.Controls.Add(Me.TableLayoutPanel1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "DialogProductCode"
        Me.Text = "DialogProductCode"
        Me.TableLayoutPanel1.ResumeLayout(False)
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents TableLayoutPanel1 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents OK_Button As System.Windows.Forms.Button
    Friend WithEvents Cancel_Button As System.Windows.Forms.Button
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents LabelProcess As System.Windows.Forms.Label
    Friend WithEvents TextProductCode As System.Windows.Forms.TextBox
    Friend WithEvents Label_Version As System.Windows.Forms.Label
    Friend WithEvents PictureBox2 As System.Windows.Forms.PictureBox
    Friend WithEvents LabelSerial As System.Windows.Forms.Label
    Friend WithEvents TextCompanyName As System.Windows.Forms.TextBox
    Friend WithEvents LabelCompanyName As System.Windows.Forms.Label

End Class

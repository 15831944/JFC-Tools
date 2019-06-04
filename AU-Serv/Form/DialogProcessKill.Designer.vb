<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class DialogProcessKill
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
        Me.Kill_Button = New System.Windows.Forms.Button()
        Me.Cancel_Button = New System.Windows.Forms.Button()
        Me.OK_Button = New System.Windows.Forms.Button()
        Me.CheckedListBoxProcessKill = New System.Windows.Forms.CheckedListBox()
        Me.LabelProcess = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.Label_Version = New System.Windows.Forms.Label()
        Me.PictureBox2 = New System.Windows.Forms.PictureBox()
        Me.PictureBox3 = New System.Windows.Forms.PictureBox()
        Me.TableLayoutPanel1.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBox3, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'TableLayoutPanel1
        '
        Me.TableLayoutPanel1.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TableLayoutPanel1.AutoSize = True
        Me.TableLayoutPanel1.ColumnCount = 3
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle())
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle())
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle())
        Me.TableLayoutPanel1.Controls.Add(Me.Kill_Button, 0, 0)
        Me.TableLayoutPanel1.Controls.Add(Me.Cancel_Button, 2, 0)
        Me.TableLayoutPanel1.Controls.Add(Me.OK_Button, 1, 0)
        Me.TableLayoutPanel1.Location = New System.Drawing.Point(194, 274)
        Me.TableLayoutPanel1.Name = "TableLayoutPanel1"
        Me.TableLayoutPanel1.RowCount = 1
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100.0!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 29.0!))
        Me.TableLayoutPanel1.Size = New System.Drawing.Size(229, 29)
        Me.TableLayoutPanel1.TabIndex = 0
        '
        'Kill_Button
        '
        Me.Kill_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.Kill_Button.AutoSize = True
        Me.Kill_Button.Enabled = False
        Me.Kill_Button.Location = New System.Drawing.Point(3, 3)
        Me.Kill_Button.Name = "Kill_Button"
        Me.Kill_Button.Size = New System.Drawing.Size(77, 23)
        Me.Kill_Button.TabIndex = 3
        Me.Kill_Button.Text = "End Process"
        '
        'Cancel_Button
        '
        Me.Cancel_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.Cancel_Button.DialogResult = System.Windows.Forms.DialogResult.No
        Me.Cancel_Button.Location = New System.Drawing.Point(159, 3)
        Me.Cancel_Button.Name = "Cancel_Button"
        Me.Cancel_Button.Size = New System.Drawing.Size(67, 23)
        Me.Cancel_Button.TabIndex = 1
        Me.Cancel_Button.Text = "No"
        '
        'OK_Button
        '
        Me.OK_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.OK_Button.DialogResult = System.Windows.Forms.DialogResult.Yes
        Me.OK_Button.Location = New System.Drawing.Point(86, 3)
        Me.OK_Button.Name = "OK_Button"
        Me.OK_Button.Size = New System.Drawing.Size(67, 23)
        Me.OK_Button.TabIndex = 2
        Me.OK_Button.Text = "Yes"
        '
        'CheckedListBoxProcessKill
        '
        Me.CheckedListBoxProcessKill.CheckOnClick = True
        Me.CheckedListBoxProcessKill.FormattingEnabled = True
        Me.CheckedListBoxProcessKill.Location = New System.Drawing.Point(12, 72)
        Me.CheckedListBoxProcessKill.Name = "CheckedListBoxProcessKill"
        Me.CheckedListBoxProcessKill.Size = New System.Drawing.Size(411, 184)
        Me.CheckedListBoxProcessKill.TabIndex = 4
        '
        'LabelProcess
        '
        Me.LabelProcess.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Bold)
        Me.LabelProcess.Location = New System.Drawing.Point(80, 9)
        Me.LabelProcess.Name = "LabelProcess"
        Me.LabelProcess.Size = New System.Drawing.Size(313, 60)
        Me.LabelProcess.TabIndex = 2
        Me.LabelProcess.Text = "Warning"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.AUServ.My.Resources.Resources.avertissement
        Me.PictureBox1.Location = New System.Drawing.Point(12, 9)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(62, 60)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBox1.TabIndex = 3
        Me.PictureBox1.TabStop = False
        '
        'Label_Version
        '
        Me.Label_Version.AutoSize = True
        Me.Label_Version.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Version.Location = New System.Drawing.Point(69, 297)
        Me.Label_Version.Name = "Label_Version"
        Me.Label_Version.Size = New System.Drawing.Size(27, 9)
        Me.Label_Version.TabIndex = 19
        Me.Label_Version.Text = "1.0.0.0"
        '
        'PictureBox2
        '
        Me.PictureBox2.Image = Global.AUServ.My.Resources.Resources.kantard
        Me.PictureBox2.Location = New System.Drawing.Point(12, 267)
        Me.PictureBox2.Name = "PictureBox2"
        Me.PictureBox2.Size = New System.Drawing.Size(84, 36)
        Me.PictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.PictureBox2.TabIndex = 18
        Me.PictureBox2.TabStop = False
        '
        'PictureBox3
        '
        Me.PictureBox3.Image = Global.AUServ.My.Resources.Resources.About
        Me.PictureBox3.Location = New System.Drawing.Point(399, 12)
        Me.PictureBox3.Name = "PictureBox3"
        Me.PictureBox3.Size = New System.Drawing.Size(24, 24)
        Me.PictureBox3.TabIndex = 20
        Me.PictureBox3.TabStop = False
        '
        'DialogProcessKill
        '
        Me.AcceptButton = Me.OK_Button
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.CancelButton = Me.Cancel_Button
        Me.ClientSize = New System.Drawing.Size(435, 315)
        Me.Controls.Add(Me.PictureBox3)
        Me.Controls.Add(Me.Label_Version)
        Me.Controls.Add(Me.PictureBox2)
        Me.Controls.Add(Me.PictureBox1)
        Me.Controls.Add(Me.LabelProcess)
        Me.Controls.Add(Me.CheckedListBoxProcessKill)
        Me.Controls.Add(Me.TableLayoutPanel1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "DialogProcessKill"
        Me.Text = "Dialog Process Kill"
        Me.TableLayoutPanel1.ResumeLayout(False)
        Me.TableLayoutPanel1.PerformLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBox3, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents TableLayoutPanel1 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents Cancel_Button As System.Windows.Forms.Button
    Friend WithEvents CheckedListBoxProcessKill As System.Windows.Forms.CheckedListBox
    Friend WithEvents Kill_Button As System.Windows.Forms.Button
    Friend WithEvents LabelProcess As System.Windows.Forms.Label
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents OK_Button As System.Windows.Forms.Button
    Friend WithEvents Label_Version As System.Windows.Forms.Label
    Friend WithEvents PictureBox2 As System.Windows.Forms.PictureBox
    Friend WithEvents PictureBox3 As System.Windows.Forms.PictureBox

End Class

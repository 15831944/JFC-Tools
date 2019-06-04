<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Settings
    Inherits System.Windows.Forms.Form

    'Form remplace la méthode Dispose pour nettoyer la liste des composants.
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

    'Requise par le Concepteur Windows Form
    Private components As System.ComponentModel.IContainer

    'REMARQUE : la procédure suivante est requise par le Concepteur Windows Form
    'Elle peut être modifiée à l'aide du Concepteur Windows Form.  
    'Ne la modifiez pas à l'aide de l'éditeur de code.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Settings))
        Me.TableLayoutPanel1 = New System.Windows.Forms.TableLayoutPanel()
        Me.OK_Button = New System.Windows.Forms.Button()
        Me.Cancel_Button = New System.Windows.Forms.Button()
        Me.CheckDirectConnect = New System.Windows.Forms.CheckBox()
        Me.CheckUpdateAuto = New System.Windows.Forms.CheckBox()
        Me.TextProxy = New System.Windows.Forms.TextBox()
        Me.LabelProxy = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.ComboAllUpdates = New System.Windows.Forms.ComboBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.TableLayoutPanel1.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
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
        Me.TableLayoutPanel1.Location = New System.Drawing.Point(165, 138)
        Me.TableLayoutPanel1.Name = "TableLayoutPanel1"
        Me.TableLayoutPanel1.RowCount = 1
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50.0!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 42.0!))
        Me.TableLayoutPanel1.Size = New System.Drawing.Size(237, 42)
        Me.TableLayoutPanel1.TabIndex = 0
        '
        'OK_Button
        '
        Me.OK_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.OK_Button.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.OK_Button.Image = Global.AUServ.My.Resources.Resources.certifie
        Me.OK_Button.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.OK_Button.Location = New System.Drawing.Point(3, 3)
        Me.OK_Button.Name = "OK_Button"
        Me.OK_Button.Size = New System.Drawing.Size(112, 36)
        Me.OK_Button.TabIndex = 0
        Me.OK_Button.Text = "   Save"
        '
        'Cancel_Button
        '
        Me.Cancel_Button.Anchor = System.Windows.Forms.AnchorStyles.None
        Me.Cancel_Button.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Cancel_Button.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Cancel_Button.Image = Global.AUServ.My.Resources.Resources.annuler
        Me.Cancel_Button.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Cancel_Button.Location = New System.Drawing.Point(121, 3)
        Me.Cancel_Button.Name = "Cancel_Button"
        Me.Cancel_Button.Size = New System.Drawing.Size(113, 36)
        Me.Cancel_Button.TabIndex = 1
        Me.Cancel_Button.Text = "   Cancel"
        '
        'CheckDirectConnect
        '
        Me.CheckDirectConnect.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CheckDirectConnect.Image = Global.AUServ.My.Resources.Resources.cable_connexion_ethernet_reseau
        Me.CheckDirectConnect.ImageAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.CheckDirectConnect.Location = New System.Drawing.Point(24, 12)
        Me.CheckDirectConnect.Name = "CheckDirectConnect"
        Me.CheckDirectConnect.Size = New System.Drawing.Size(152, 33)
        Me.CheckDirectConnect.TabIndex = 1
        Me.CheckDirectConnect.Text = "Direct Connect      "
        Me.CheckDirectConnect.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.CheckDirectConnect.UseVisualStyleBackColor = True
        '
        'CheckUpdateAuto
        '
        Me.CheckUpdateAuto.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CheckUpdateAuto.Image = Global.AUServ.My.Resources.Resources.automatiques_mises_a_jour
        Me.CheckUpdateAuto.ImageAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.CheckUpdateAuto.Location = New System.Drawing.Point(24, 51)
        Me.CheckUpdateAuto.Name = "CheckUpdateAuto"
        Me.CheckUpdateAuto.Size = New System.Drawing.Size(164, 33)
        Me.CheckUpdateAuto.TabIndex = 7
        Me.CheckUpdateAuto.Text = "Automatic update    "
        Me.CheckUpdateAuto.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.CheckUpdateAuto.UseVisualStyleBackColor = True
        '
        'TextProxy
        '
        Me.TextProxy.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextProxy.Location = New System.Drawing.Point(102, 92)
        Me.TextProxy.Name = "TextProxy"
        Me.TextProxy.Size = New System.Drawing.Size(297, 22)
        Me.TextProxy.TabIndex = 8
        '
        'LabelProxy
        '
        Me.LabelProxy.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelProxy.Image = CType(resources.GetObject("LabelProxy.Image"), System.Drawing.Image)
        Me.LabelProxy.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.LabelProxy.Location = New System.Drawing.Point(21, 87)
        Me.LabelProxy.Name = "LabelProxy"
        Me.LabelProxy.Size = New System.Drawing.Size(75, 33)
        Me.LabelProxy.TabIndex = 9
        Me.LabelProxy.Text = "Proxy:"
        Me.LabelProxy.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.AUServ.My.Resources.Resources.kantard
        Me.PictureBox1.Location = New System.Drawing.Point(12, 138)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(147, 42)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.PictureBox1.TabIndex = 14
        Me.PictureBox1.TabStop = False
        '
        'ComboAllUpdates
        '
        Me.ComboAllUpdates.FormattingEnabled = True
        Me.ComboAllUpdates.Location = New System.Drawing.Point(230, 28)
        Me.ComboAllUpdates.Name = "ComboAllUpdates"
        Me.ComboAllUpdates.Size = New System.Drawing.Size(172, 21)
        Me.ComboAllUpdates.TabIndex = 16
        '
        'Label1
        '
        Me.Label1.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(230, 9)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(172, 16)
        Me.Label1.TabIndex = 17
        Me.Label1.Text = "Synchronizes"
        '
        'Settings
        '
        Me.AcceptButton = Me.OK_Button
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.CancelButton = Me.Cancel_Button
        Me.ClientSize = New System.Drawing.Size(414, 192)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ComboAllUpdates)
        Me.Controls.Add(Me.PictureBox1)
        Me.Controls.Add(Me.LabelProxy)
        Me.Controls.Add(Me.TextProxy)
        Me.Controls.Add(Me.CheckUpdateAuto)
        Me.Controls.Add(Me.CheckDirectConnect)
        Me.Controls.Add(Me.TableLayoutPanel1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "Settings"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Settings"
        Me.TableLayoutPanel1.ResumeLayout(False)
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents TableLayoutPanel1 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents OK_Button As System.Windows.Forms.Button
    Friend WithEvents Cancel_Button As System.Windows.Forms.Button
    Friend WithEvents CheckDirectConnect As System.Windows.Forms.CheckBox
    Friend WithEvents CheckUpdateAuto As System.Windows.Forms.CheckBox
    Friend WithEvents TextProxy As System.Windows.Forms.TextBox
    Friend WithEvents LabelProxy As System.Windows.Forms.Label
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents ComboAllUpdates As System.Windows.Forms.ComboBox
    Friend WithEvents Label1 As System.Windows.Forms.Label

End Class

<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class CreateUpdate
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(CreateUpdate))
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ComboBoxPatchName = New System.Windows.Forms.ComboBox()
        Me.TextPatchInformation = New System.Windows.Forms.TextBox()
        Me.OpenFileDialogVersion = New System.Windows.Forms.OpenFileDialog()
        Me.ButtonGetTarifs = New System.Windows.Forms.Button()
        Me.ButtonPigeAuto = New System.Windows.Forms.Button()
        Me.ButtonGetVersion = New System.Windows.Forms.Button()
        Me.ButtonGetData = New System.Windows.Forms.Button()
        Me.ButtonCreating = New System.Windows.Forms.Button()
        Me.ButtonExit = New System.Windows.Forms.Button()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.ToolStripProgressBar1 = New System.Windows.Forms.ToolStripProgressBar()
        Me.ToolStripStatusLabel1 = New System.Windows.Forms.ToolStripStatusLabel()
        Me.OpenFileDialog1 = New System.Windows.Forms.OpenFileDialog()
        Me.CheckBoxExe = New System.Windows.Forms.CheckBox()
        Me.CheckBoxZip = New System.Windows.Forms.CheckBox()
        Me.StatusStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(10, 39)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(108, 13)
        Me.Label2.TabIndex = 13
        Me.Label2.Text = "Information Package:"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(10, 12)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(82, 13)
        Me.Label1.TabIndex = 12
        Me.Label1.Text = "Package name:"
        '
        'ComboBoxPatchName
        '
        Me.ComboBoxPatchName.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ComboBoxPatchName.FormattingEnabled = True
        Me.ComboBoxPatchName.Location = New System.Drawing.Point(124, 12)
        Me.ComboBoxPatchName.Name = "ComboBoxPatchName"
        Me.ComboBoxPatchName.Size = New System.Drawing.Size(248, 21)
        Me.ComboBoxPatchName.TabIndex = 11
        '
        'TextPatchInformation
        '
        Me.TextPatchInformation.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TextPatchInformation.Location = New System.Drawing.Point(123, 39)
        Me.TextPatchInformation.Multiline = True
        Me.TextPatchInformation.Name = "TextPatchInformation"
        Me.TextPatchInformation.Size = New System.Drawing.Size(249, 193)
        Me.TextPatchInformation.TabIndex = 10
        '
        'OpenFileDialogVersion
        '
        Me.OpenFileDialogVersion.FileName = "OpenFileDialog1"
        '
        'ButtonGetTarifs
        '
        Me.ButtonGetTarifs.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.ButtonGetTarifs.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonGetTarifs.Image = Global.Facility_Management.My.Resources.Resources.Tarifs
        Me.ButtonGetTarifs.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonGetTarifs.Location = New System.Drawing.Point(12, 103)
        Me.ButtonGetTarifs.Name = "ButtonGetTarifs"
        Me.ButtonGetTarifs.Size = New System.Drawing.Size(105, 39)
        Me.ButtonGetTarifs.TabIndex = 23
        Me.ButtonGetTarifs.Text = "Tarifs     "
        Me.ButtonGetTarifs.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonGetTarifs.UseVisualStyleBackColor = True
        '
        'ButtonPigeAuto
        '
        Me.ButtonPigeAuto.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.ButtonPigeAuto.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonPigeAuto.Image = Global.Facility_Management.My.Resources.Resources.Wizard_Wand
        Me.ButtonPigeAuto.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonPigeAuto.Location = New System.Drawing.Point(12, 58)
        Me.ButtonPigeAuto.Name = "ButtonPigeAuto"
        Me.ButtonPigeAuto.Size = New System.Drawing.Size(105, 39)
        Me.ButtonPigeAuto.TabIndex = 22
        Me.ButtonPigeAuto.Text = "Pige Auto"
        Me.ButtonPigeAuto.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonPigeAuto.UseVisualStyleBackColor = True
        '
        'ButtonGetVersion
        '
        Me.ButtonGetVersion.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.ButtonGetVersion.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonGetVersion.Image = Global.Facility_Management.My.Resources.Resources.Copy_to_Folder
        Me.ButtonGetVersion.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonGetVersion.Location = New System.Drawing.Point(12, 148)
        Me.ButtonGetVersion.Name = "ButtonGetVersion"
        Me.ButtonGetVersion.Size = New System.Drawing.Size(105, 39)
        Me.ButtonGetVersion.TabIndex = 21
        Me.ButtonGetVersion.Text = "Version  "
        Me.ButtonGetVersion.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonGetVersion.UseVisualStyleBackColor = True
        '
        'ButtonGetData
        '
        Me.ButtonGetData.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.ButtonGetData.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonGetData.Image = Global.Facility_Management.My.Resources.Resources.ImportData
        Me.ButtonGetData.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonGetData.Location = New System.Drawing.Point(12, 193)
        Me.ButtonGetData.Name = "ButtonGetData"
        Me.ButtonGetData.Size = New System.Drawing.Size(105, 39)
        Me.ButtonGetData.TabIndex = 17
        Me.ButtonGetData.Text = "Sources "
        Me.ButtonGetData.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonGetData.UseVisualStyleBackColor = True
        '
        'ButtonCreating
        '
        Me.ButtonCreating.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ButtonCreating.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonCreating.Image = Global.Facility_Management.My.Resources.Resources.Package2
        Me.ButtonCreating.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonCreating.Location = New System.Drawing.Point(158, 238)
        Me.ButtonCreating.Name = "ButtonCreating"
        Me.ButtonCreating.Size = New System.Drawing.Size(104, 39)
        Me.ButtonCreating.TabIndex = 14
        Me.ButtonCreating.Text = "Creating"
        Me.ButtonCreating.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonCreating.UseVisualStyleBackColor = True
        '
        'ButtonExit
        '
        Me.ButtonExit.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ButtonExit.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonExit.Image = Global.Facility_Management.My.Resources.Resources.sortie
        Me.ButtonExit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonExit.Location = New System.Drawing.Point(268, 238)
        Me.ButtonExit.Name = "ButtonExit"
        Me.ButtonExit.Size = New System.Drawing.Size(104, 39)
        Me.ButtonExit.TabIndex = 9
        Me.ButtonExit.Text = "Exit    "
        Me.ButtonExit.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonExit.UseVisualStyleBackColor = True
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ToolStripProgressBar1, Me.ToolStripStatusLabel1})
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 280)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Size = New System.Drawing.Size(384, 22)
        Me.StatusStrip1.TabIndex = 24
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'ToolStripProgressBar1
        '
        Me.ToolStripProgressBar1.MarqueeAnimationSpeed = 50
        Me.ToolStripProgressBar1.Name = "ToolStripProgressBar1"
        Me.ToolStripProgressBar1.Size = New System.Drawing.Size(100, 16)
        '
        'ToolStripStatusLabel1
        '
        Me.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1"
        Me.ToolStripStatusLabel1.Size = New System.Drawing.Size(0, 17)
        '
        'OpenFileDialog1
        '
        Me.OpenFileDialog1.FileName = "OpenFileDialog1"
        '
        'CheckBoxExe
        '
        Me.CheckBoxExe.AutoSize = True
        Me.CheckBoxExe.Location = New System.Drawing.Point(12, 260)
        Me.CheckBoxExe.Name = "CheckBoxExe"
        Me.CheckBoxExe.Size = New System.Drawing.Size(96, 17)
        Me.CheckBoxExe.TabIndex = 27
        Me.CheckBoxExe.Text = "EXE Extension"
        Me.CheckBoxExe.UseVisualStyleBackColor = True
        '
        'CheckBoxZip
        '
        Me.CheckBoxZip.AutoSize = True
        Me.CheckBoxZip.Location = New System.Drawing.Point(12, 237)
        Me.CheckBoxZip.Name = "CheckBoxZip"
        Me.CheckBoxZip.Size = New System.Drawing.Size(92, 17)
        Me.CheckBoxZip.TabIndex = 28
        Me.CheckBoxZip.Text = "ZIP Extension"
        Me.CheckBoxZip.UseVisualStyleBackColor = True
        '
        'CreateUpdate
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(384, 302)
        Me.Controls.Add(Me.CheckBoxZip)
        Me.Controls.Add(Me.CheckBoxExe)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.ButtonGetTarifs)
        Me.Controls.Add(Me.ButtonPigeAuto)
        Me.Controls.Add(Me.ButtonGetVersion)
        Me.Controls.Add(Me.ButtonGetData)
        Me.Controls.Add(Me.ButtonCreating)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ComboBoxPatchName)
        Me.Controls.Add(Me.TextPatchInformation)
        Me.Controls.Add(Me.ButtonExit)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.KeyPreview = True
        Me.MaximizeBox = False
        Me.MinimumSize = New System.Drawing.Size(365, 340)
        Me.Name = "CreateUpdate"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Create Patch"
        Me.StatusStrip1.ResumeLayout(False)
        Me.StatusStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents ButtonCreating As System.Windows.Forms.Button
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents ComboBoxPatchName As System.Windows.Forms.ComboBox
    Friend WithEvents TextPatchInformation As System.Windows.Forms.TextBox
    Friend WithEvents ButtonExit As System.Windows.Forms.Button
    Friend WithEvents ButtonGetData As System.Windows.Forms.Button
    Friend WithEvents OpenFileDialogVersion As System.Windows.Forms.OpenFileDialog
    Friend WithEvents ButtonGetVersion As System.Windows.Forms.Button
    Friend WithEvents ButtonPigeAuto As System.Windows.Forms.Button
    Friend WithEvents ButtonGetTarifs As System.Windows.Forms.Button
    Friend WithEvents StatusStrip1 As System.Windows.Forms.StatusStrip
    Friend WithEvents ToolStripProgressBar1 As System.Windows.Forms.ToolStripProgressBar
    Friend WithEvents ToolStripStatusLabel1 As System.Windows.Forms.ToolStripStatusLabel
    Friend WithEvents OpenFileDialog1 As System.Windows.Forms.OpenFileDialog
    Friend WithEvents CheckBoxExe As CheckBox
    Friend WithEvents CheckBoxZip As CheckBox
End Class

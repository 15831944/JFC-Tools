<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Archivage
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Archivage))
        Me.DateTimeStart = New System.Windows.Forms.DateTimePicker()
        Me.DateTimeEnd = New System.Windows.Forms.DateTimePicker()
        Me.ButtonStart = New System.Windows.Forms.Button()
        Me.LabelStart = New System.Windows.Forms.Label()
        Me.LabelEnd = New System.Windows.Forms.Label()
        Me.ListBoxEtudes = New System.Windows.Forms.ListBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.CheckBoxUfr14 = New System.Windows.Forms.CheckBox()
        Me.CheckBoxUfr15 = New System.Windows.Forms.CheckBox()
        Me.CheckBoxUfr09 = New System.Windows.Forms.CheckBox()
        Me.CheckBoxUfr10 = New System.Windows.Forms.CheckBox()
        Me.CheckBoxUfr03 = New System.Windows.Forms.CheckBox()
        Me.CheckBoxUfr02 = New System.Windows.Forms.CheckBox()
        Me.ButtonQuitter = New System.Windows.Forms.Button()
        Me.LabelPath = New System.Windows.Forms.Label()
        Me.ButtonExecute = New System.Windows.Forms.Button()
        Me.LabelArchives = New System.Windows.Forms.Label()
        Me.ComboBoxArchives = New System.Windows.Forms.ComboBox()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.OptionsToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.ModeRestaurationToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.EclusFichierLectureSeuleToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.AjoutÉtudesÉrronéesToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.TriCroissantToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.ModeTestChargementToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.AProposToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.LabelEmplacement = New System.Windows.Forms.Label()
        Me.ListBoxPlan = New System.Windows.Forms.ListBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Rapide = New System.Windows.Forms.Button()
        Me.GroupBox1.SuspendLayout()
        Me.MenuStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'DateTimeStart
        '
        Me.DateTimeStart.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.DateTimeStart.Format = System.Windows.Forms.DateTimePickerFormat.[Short]
        Me.DateTimeStart.Location = New System.Drawing.Point(121, 42)
        Me.DateTimeStart.Name = "DateTimeStart"
        Me.DateTimeStart.Size = New System.Drawing.Size(120, 24)
        Me.DateTimeStart.TabIndex = 1
        Me.DateTimeStart.Value = New Date(2008, 1, 1, 12, 0, 0, 0)
        '
        'DateTimeEnd
        '
        Me.DateTimeEnd.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.DateTimeEnd.Format = System.Windows.Forms.DateTimePickerFormat.[Short]
        Me.DateTimeEnd.Location = New System.Drawing.Point(121, 72)
        Me.DateTimeEnd.Name = "DateTimeEnd"
        Me.DateTimeEnd.Size = New System.Drawing.Size(120, 24)
        Me.DateTimeEnd.TabIndex = 2
        '
        'ButtonStart
        '
        Me.ButtonStart.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonStart.Image = CType(resources.GetObject("ButtonStart.Image"), System.Drawing.Image)
        Me.ButtonStart.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonStart.Location = New System.Drawing.Point(775, 585)
        Me.ButtonStart.Name = "ButtonStart"
        Me.ButtonStart.Size = New System.Drawing.Size(107, 39)
        Me.ButtonStart.TabIndex = 0
        Me.ButtonStart.Text = "Charger"
        Me.ButtonStart.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonStart.UseVisualStyleBackColor = True
        '
        'LabelStart
        '
        Me.LabelStart.AutoSize = True
        Me.LabelStart.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelStart.Location = New System.Drawing.Point(12, 48)
        Me.LabelStart.Name = "LabelStart"
        Me.LabelStart.Size = New System.Drawing.Size(103, 18)
        Me.LabelStart.TabIndex = 3
        Me.LabelStart.Text = "Date de début:"
        '
        'LabelEnd
        '
        Me.LabelEnd.AutoSize = True
        Me.LabelEnd.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelEnd.Location = New System.Drawing.Point(12, 78)
        Me.LabelEnd.Name = "LabelEnd"
        Me.LabelEnd.Size = New System.Drawing.Size(82, 18)
        Me.LabelEnd.TabIndex = 4
        Me.LabelEnd.Text = "Date de fin:"
        '
        'ListBoxEtudes
        '
        Me.ListBoxEtudes.Font = New System.Drawing.Font("Courier New", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ListBoxEtudes.FormattingEnabled = True
        Me.ListBoxEtudes.HorizontalScrollbar = True
        Me.ListBoxEtudes.ItemHeight = 14
        Me.ListBoxEtudes.Location = New System.Drawing.Point(12, 99)
        Me.ListBoxEtudes.Name = "ListBoxEtudes"
        Me.ListBoxEtudes.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended
        Me.ListBoxEtudes.Size = New System.Drawing.Size(770, 480)
        Me.ListBoxEtudes.TabIndex = 4
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.CheckBoxUfr14)
        Me.GroupBox1.Controls.Add(Me.CheckBoxUfr15)
        Me.GroupBox1.Controls.Add(Me.CheckBoxUfr09)
        Me.GroupBox1.Controls.Add(Me.CheckBoxUfr10)
        Me.GroupBox1.Controls.Add(Me.CheckBoxUfr03)
        Me.GroupBox1.Controls.Add(Me.CheckBoxUfr02)
        Me.GroupBox1.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GroupBox1.Location = New System.Drawing.Point(271, 43)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(511, 50)
        Me.GroupBox1.TabIndex = 3
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Univers"
        '
        'CheckBoxUfr14
        '
        Me.CheckBoxUfr14.AutoSize = True
        Me.CheckBoxUfr14.Location = New System.Drawing.Point(335, 20)
        Me.CheckBoxUfr14.Name = "CheckBoxUfr14"
        Me.CheckBoxUfr14.Size = New System.Drawing.Size(74, 22)
        Me.CheckBoxUfr14.TabIndex = 4
        Me.CheckBoxUfr14.Text = "UFR14"
        Me.CheckBoxUfr14.UseVisualStyleBackColor = True
        '
        'CheckBoxUfr15
        '
        Me.CheckBoxUfr15.AutoSize = True
        Me.CheckBoxUfr15.Location = New System.Drawing.Point(415, 20)
        Me.CheckBoxUfr15.Name = "CheckBoxUfr15"
        Me.CheckBoxUfr15.Size = New System.Drawing.Size(74, 22)
        Me.CheckBoxUfr15.TabIndex = 5
        Me.CheckBoxUfr15.Text = "UFR15"
        Me.CheckBoxUfr15.UseVisualStyleBackColor = True
        '
        'CheckBoxUfr09
        '
        Me.CheckBoxUfr09.AutoSize = True
        Me.CheckBoxUfr09.Location = New System.Drawing.Point(175, 20)
        Me.CheckBoxUfr09.Name = "CheckBoxUfr09"
        Me.CheckBoxUfr09.Size = New System.Drawing.Size(74, 22)
        Me.CheckBoxUfr09.TabIndex = 2
        Me.CheckBoxUfr09.Text = "UFR09"
        Me.CheckBoxUfr09.UseVisualStyleBackColor = True
        '
        'CheckBoxUfr10
        '
        Me.CheckBoxUfr10.AutoSize = True
        Me.CheckBoxUfr10.Location = New System.Drawing.Point(255, 20)
        Me.CheckBoxUfr10.Name = "CheckBoxUfr10"
        Me.CheckBoxUfr10.Size = New System.Drawing.Size(74, 22)
        Me.CheckBoxUfr10.TabIndex = 3
        Me.CheckBoxUfr10.Text = "UFR10"
        Me.CheckBoxUfr10.UseVisualStyleBackColor = True
        '
        'CheckBoxUfr03
        '
        Me.CheckBoxUfr03.AutoSize = True
        Me.CheckBoxUfr03.Location = New System.Drawing.Point(95, 20)
        Me.CheckBoxUfr03.Name = "CheckBoxUfr03"
        Me.CheckBoxUfr03.Size = New System.Drawing.Size(74, 22)
        Me.CheckBoxUfr03.TabIndex = 1
        Me.CheckBoxUfr03.Text = "UFR03"
        Me.CheckBoxUfr03.UseVisualStyleBackColor = True
        '
        'CheckBoxUfr02
        '
        Me.CheckBoxUfr02.AutoSize = True
        Me.CheckBoxUfr02.Location = New System.Drawing.Point(15, 20)
        Me.CheckBoxUfr02.Name = "CheckBoxUfr02"
        Me.CheckBoxUfr02.Size = New System.Drawing.Size(74, 22)
        Me.CheckBoxUfr02.TabIndex = 0
        Me.CheckBoxUfr02.Text = "UFR02"
        Me.CheckBoxUfr02.UseVisualStyleBackColor = True
        '
        'ButtonQuitter
        '
        Me.ButtonQuitter.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonQuitter.Image = CType(resources.GetObject("ButtonQuitter.Image"), System.Drawing.Image)
        Me.ButtonQuitter.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonQuitter.Location = New System.Drawing.Point(888, 585)
        Me.ButtonQuitter.Name = "ButtonQuitter"
        Me.ButtonQuitter.Size = New System.Drawing.Size(109, 39)
        Me.ButtonQuitter.TabIndex = 5
        Me.ButtonQuitter.Text = "Quitter"
        Me.ButtonQuitter.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonQuitter.UseVisualStyleBackColor = True
        '
        'LabelPath
        '
        Me.LabelPath.AutoSize = True
        Me.LabelPath.ForeColor = System.Drawing.Color.Blue
        Me.LabelPath.Location = New System.Drawing.Point(12, 614)
        Me.LabelPath.Name = "LabelPath"
        Me.LabelPath.Size = New System.Drawing.Size(56, 13)
        Me.LabelPath.TabIndex = 6
        Me.LabelPath.Text = "path users"
        '
        'ButtonExecute
        '
        Me.ButtonExecute.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonExecute.Image = CType(resources.GetObject("ButtonExecute.Image"), System.Drawing.Image)
        Me.ButtonExecute.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.ButtonExecute.Location = New System.Drawing.Point(660, 585)
        Me.ButtonExecute.Name = "ButtonExecute"
        Me.ButtonExecute.Size = New System.Drawing.Size(109, 39)
        Me.ButtonExecute.TabIndex = 7
        Me.ButtonExecute.Text = "Executer"
        Me.ButtonExecute.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.ButtonExecute.UseVisualStyleBackColor = True
        '
        'LabelArchives
        '
        Me.LabelArchives.AutoSize = True
        Me.LabelArchives.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelArchives.Location = New System.Drawing.Point(12, 40)
        Me.LabelArchives.Name = "LabelArchives"
        Me.LabelArchives.Size = New System.Drawing.Size(130, 18)
        Me.LabelArchives.TabIndex = 13
        Me.LabelArchives.Text = "Liste des archives:"
        '
        'ComboBoxArchives
        '
        Me.ComboBoxArchives.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxArchives.FormattingEnabled = True
        Me.ComboBoxArchives.Location = New System.Drawing.Point(12, 61)
        Me.ComboBoxArchives.Name = "ComboBoxArchives"
        Me.ComboBoxArchives.Size = New System.Drawing.Size(253, 26)
        Me.ComboBoxArchives.TabIndex = 12
        '
        'MenuStrip1
        '
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.OptionsToolStripMenuItem, Me.AProposToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(1009, 40)
        Me.MenuStrip1.TabIndex = 17
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'OptionsToolStripMenuItem
        '
        Me.OptionsToolStripMenuItem.Checked = True
        Me.OptionsToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked
        Me.OptionsToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ModeRestaurationToolStripMenuItem, Me.EclusFichierLectureSeuleToolStripMenuItem, Me.AjoutÉtudesÉrronéesToolStripMenuItem, Me.TriCroissantToolStripMenuItem, Me.ModeTestChargementToolStripMenuItem})
        Me.OptionsToolStripMenuItem.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.OptionsToolStripMenuItem.Image = CType(resources.GetObject("OptionsToolStripMenuItem.Image"), System.Drawing.Image)
        Me.OptionsToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.OptionsToolStripMenuItem.Name = "OptionsToolStripMenuItem"
        Me.OptionsToolStripMenuItem.Size = New System.Drawing.Size(108, 36)
        Me.OptionsToolStripMenuItem.Text = "Options"
        '
        'ModeRestaurationToolStripMenuItem
        '
        Me.ModeRestaurationToolStripMenuItem.Image = CType(resources.GetObject("ModeRestaurationToolStripMenuItem.Image"), System.Drawing.Image)
        Me.ModeRestaurationToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.ModeRestaurationToolStripMenuItem.Name = "ModeRestaurationToolStripMenuItem"
        Me.ModeRestaurationToolStripMenuItem.Size = New System.Drawing.Size(340, 38)
        Me.ModeRestaurationToolStripMenuItem.Text = "Mode restauration"
        '
        'EclusFichierLectureSeuleToolStripMenuItem
        '
        Me.EclusFichierLectureSeuleToolStripMenuItem.Image = CType(resources.GetObject("EclusFichierLectureSeuleToolStripMenuItem.Image"), System.Drawing.Image)
        Me.EclusFichierLectureSeuleToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.EclusFichierLectureSeuleToolStripMenuItem.Name = "EclusFichierLectureSeuleToolStripMenuItem"
        Me.EclusFichierLectureSeuleToolStripMenuItem.Size = New System.Drawing.Size(340, 38)
        Me.EclusFichierLectureSeuleToolStripMenuItem.Text = "Exclure les fichiers en lecture seule"
        '
        'AjoutÉtudesÉrronéesToolStripMenuItem
        '
        Me.AjoutÉtudesÉrronéesToolStripMenuItem.Image = CType(resources.GetObject("AjoutÉtudesÉrronéesToolStripMenuItem.Image"), System.Drawing.Image)
        Me.AjoutÉtudesÉrronéesToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.AjoutÉtudesÉrronéesToolStripMenuItem.Name = "AjoutÉtudesÉrronéesToolStripMenuItem"
        Me.AjoutÉtudesÉrronéesToolStripMenuItem.Size = New System.Drawing.Size(340, 38)
        Me.AjoutÉtudesÉrronéesToolStripMenuItem.Text = "Ajouter les études érronées"
        '
        'TriCroissantToolStripMenuItem
        '
        Me.TriCroissantToolStripMenuItem.Image = CType(resources.GetObject("TriCroissantToolStripMenuItem.Image"), System.Drawing.Image)
        Me.TriCroissantToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.TriCroissantToolStripMenuItem.Name = "TriCroissantToolStripMenuItem"
        Me.TriCroissantToolStripMenuItem.Size = New System.Drawing.Size(340, 38)
        Me.TriCroissantToolStripMenuItem.Text = "Tri croissant"
        '
        'ModeTestChargementToolStripMenuItem
        '
        Me.ModeTestChargementToolStripMenuItem.Image = CType(resources.GetObject("ModeTestChargementToolStripMenuItem.Image"), System.Drawing.Image)
        Me.ModeTestChargementToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.ModeTestChargementToolStripMenuItem.Name = "ModeTestChargementToolStripMenuItem"
        Me.ModeTestChargementToolStripMenuItem.Size = New System.Drawing.Size(340, 38)
        Me.ModeTestChargementToolStripMenuItem.Text = "Mode test de chargement"
        '
        'AProposToolStripMenuItem
        '
        Me.AProposToolStripMenuItem.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.AProposToolStripMenuItem.Image = CType(resources.GetObject("AProposToolStripMenuItem.Image"), System.Drawing.Image)
        Me.AProposToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None
        Me.AProposToolStripMenuItem.Name = "AProposToolStripMenuItem"
        Me.AProposToolStripMenuItem.Size = New System.Drawing.Size(117, 36)
        Me.AProposToolStripMenuItem.Text = "A propos"
        '
        'LabelEmplacement
        '
        Me.LabelEmplacement.AutoSize = True
        Me.LabelEmplacement.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelEmplacement.Location = New System.Drawing.Point(12, 595)
        Me.LabelEmplacement.Name = "LabelEmplacement"
        Me.LabelEmplacement.Size = New System.Drawing.Size(266, 18)
        Me.LabelEmplacement.TabIndex = 18
        Me.LabelEmplacement.Text = "Emplacement des données utilisateurs:"
        '
        'ListBoxPlan
        '
        Me.ListBoxPlan.Font = New System.Drawing.Font("Courier New", 8.25!)
        Me.ListBoxPlan.FormattingEnabled = True
        Me.ListBoxPlan.HorizontalScrollbar = True
        Me.ListBoxPlan.ItemHeight = 14
        Me.ListBoxPlan.Location = New System.Drawing.Point(788, 99)
        Me.ListBoxPlan.Name = "ListBoxPlan"
        Me.ListBoxPlan.ScrollAlwaysVisible = True
        Me.ListBoxPlan.SelectionMode = System.Windows.Forms.SelectionMode.None
        Me.ListBoxPlan.Size = New System.Drawing.Size(209, 480)
        Me.ListBoxPlan.TabIndex = 19
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(788, 77)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(196, 18)
        Me.Label1.TabIndex = 20
        Me.Label1.Text = "Information complémentaire:"
        '
        'Rapide
        '
        Me.Rapide.BackColor = System.Drawing.Color.FromArgb(CType(CType(128, Byte), Integer), CType(CType(255, Byte), Integer), CType(CType(128, Byte), Integer))
        Me.Rapide.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, CType((System.Drawing.FontStyle.Bold Or System.Drawing.FontStyle.Italic), System.Drawing.FontStyle), System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Rapide.Image = Global.Atelier_Radio_Archivage.My.Resources.Resources.Symbol_Blue_Play
        Me.Rapide.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Rapide.Location = New System.Drawing.Point(551, 585)
        Me.Rapide.Name = "Rapide"
        Me.Rapide.Size = New System.Drawing.Size(103, 39)
        Me.Rapide.TabIndex = 21
        Me.Rapide.Text = "&Rapide"
        Me.Rapide.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.Rapide.UseVisualStyleBackColor = False
        '
        'Archivage
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1009, 636)
        Me.Controls.Add(Me.Rapide)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ListBoxPlan)
        Me.Controls.Add(Me.LabelEmplacement)
        Me.Controls.Add(Me.MenuStrip1)
        Me.Controls.Add(Me.ButtonExecute)
        Me.Controls.Add(Me.LabelPath)
        Me.Controls.Add(Me.ButtonQuitter)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.ListBoxEtudes)
        Me.Controls.Add(Me.LabelEnd)
        Me.Controls.Add(Me.LabelStart)
        Me.Controls.Add(Me.ButtonStart)
        Me.Controls.Add(Me.DateTimeEnd)
        Me.Controls.Add(Me.DateTimeStart)
        Me.Controls.Add(Me.LabelArchives)
        Me.Controls.Add(Me.ComboBoxArchives)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.MaximizeBox = False
        Me.Name = "Archivage"
        Me.Text = "Atelier Radio - Archivage"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents DateTimeStart As System.Windows.Forms.DateTimePicker
    Friend WithEvents DateTimeEnd As System.Windows.Forms.DateTimePicker
    Friend WithEvents ButtonStart As System.Windows.Forms.Button
    Friend WithEvents LabelStart As System.Windows.Forms.Label
    Friend WithEvents LabelEnd As System.Windows.Forms.Label
    Friend WithEvents ListBoxEtudes As System.Windows.Forms.ListBox
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBoxUfr14 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBoxUfr15 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBoxUfr09 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBoxUfr10 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBoxUfr03 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBoxUfr02 As System.Windows.Forms.CheckBox
    Friend WithEvents ButtonQuitter As System.Windows.Forms.Button
    Friend WithEvents LabelPath As System.Windows.Forms.Label
    Friend WithEvents ButtonExecute As System.Windows.Forms.Button
    Friend WithEvents LabelArchives As System.Windows.Forms.Label
    Friend WithEvents ComboBoxArchives As System.Windows.Forms.ComboBox
    Friend WithEvents MenuStrip1 As System.Windows.Forms.MenuStrip
    Friend WithEvents OptionsToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ModeRestaurationToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents EclusFichierLectureSeuleToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents AjoutÉtudesÉrronéesToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents TriCroissantToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents AProposToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents LabelEmplacement As System.Windows.Forms.Label
    Friend WithEvents ListBoxPlan As System.Windows.Forms.ListBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents ModeTestChargementToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents Rapide As System.Windows.Forms.Button

End Class

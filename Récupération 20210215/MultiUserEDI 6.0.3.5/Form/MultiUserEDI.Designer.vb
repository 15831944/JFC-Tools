<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class MultiUserEDI
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(MultiUserEDI))
        Me.ListUsers = New System.Windows.Forms.ListBox()
        Me.LabelVeuillez = New System.Windows.Forms.Label()
        Me.GroupBoxEDI = New System.Windows.Forms.GroupBox()
        Me.RadioButton_AssetsEDI = New System.Windows.Forms.RadioButton()
        Me.Button_Param = New System.Windows.Forms.Button()
        Me.RadioButton_Nothing = New System.Windows.Forms.RadioButton()
        Me.RadioButton_UsersAssets = New System.Windows.Forms.RadioButton()
        Me.RadioButton_AllCompany = New System.Windows.Forms.RadioButton()
        Me.RadioButton_FolderEDI = New System.Windows.Forms.RadioButton()
        Me.CheckedListBox_UsersEDI = New System.Windows.Forms.CheckedListBox()
        Me.Button_Cancel = New System.Windows.Forms.Button()
        Me.Button_Valid = New System.Windows.Forms.Button()
        Me.Button_Quitter = New System.Windows.Forms.Button()
        Me.Button_Launch = New System.Windows.Forms.Button()
        Me.Label_Version = New System.Windows.Forms.Label()
        Me.GroupBoxEDI.SuspendLayout()
        Me.SuspendLayout()
        '
        'ListUsers
        '
        Me.ListUsers.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ListUsers.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.ListUsers.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ListUsers.FormattingEnabled = True
        Me.ListUsers.IntegralHeight = False
        Me.ListUsers.ItemHeight = 18
        Me.ListUsers.Location = New System.Drawing.Point(12, 30)
        Me.ListUsers.Name = "ListUsers"
        Me.ListUsers.Size = New System.Drawing.Size(218, 292)
        Me.ListUsers.TabIndex = 0
        '
        'LabelVeuillez
        '
        Me.LabelVeuillez.AutoSize = True
        Me.LabelVeuillez.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelVeuillez.Location = New System.Drawing.Point(12, 9)
        Me.LabelVeuillez.Name = "LabelVeuillez"
        Me.LabelVeuillez.Size = New System.Drawing.Size(223, 18)
        Me.LabelVeuillez.TabIndex = 3
        Me.LabelVeuillez.Text = "Veuillez choisir un environement:"
        '
        'GroupBoxEDI
        '
        Me.GroupBoxEDI.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBoxEDI.Controls.Add(Me.RadioButton_AssetsEDI)
        Me.GroupBoxEDI.Controls.Add(Me.Button_Param)
        Me.GroupBoxEDI.Controls.Add(Me.RadioButton_Nothing)
        Me.GroupBoxEDI.Controls.Add(Me.RadioButton_UsersAssets)
        Me.GroupBoxEDI.Controls.Add(Me.RadioButton_AllCompany)
        Me.GroupBoxEDI.Controls.Add(Me.RadioButton_FolderEDI)
        Me.GroupBoxEDI.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GroupBoxEDI.Location = New System.Drawing.Point(236, 130)
        Me.GroupBoxEDI.Name = "GroupBoxEDI"
        Me.GroupBoxEDI.Size = New System.Drawing.Size(139, 193)
        Me.GroupBoxEDI.TabIndex = 10
        Me.GroupBoxEDI.TabStop = False
        Me.GroupBoxEDI.Text = "Options retour EDI:"
        '
        'RadioButton_AssetsEDI
        '
        Me.RadioButton_AssetsEDI.AutoSize = True
        Me.RadioButton_AssetsEDI.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.RadioButton_AssetsEDI.Location = New System.Drawing.Point(6, 121)
        Me.RadioButton_AssetsEDI.Name = "RadioButton_AssetsEDI"
        Me.RadioButton_AssetsEDI.Size = New System.Drawing.Size(120, 19)
        Me.RadioButton_AssetsEDI.TabIndex = 17
        Me.RadioButton_AssetsEDI.TabStop = True
        Me.RadioButton_AssetsEDI.Text = "Actifs et réception"
        Me.RadioButton_AssetsEDI.UseVisualStyleBackColor = True
        '
        'Button_Param
        '
        Me.Button_Param.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Param.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button_Param.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Param.Image = Global.MultiUserEDI.My.Resources.Resources.telecharger_icone_4254_32
        Me.Button_Param.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Param.Location = New System.Drawing.Point(6, 146)
        Me.Button_Param.Name = "Button_Param"
        Me.Button_Param.Size = New System.Drawing.Size(127, 41)
        Me.Button_Param.TabIndex = 16
        Me.Button_Param.Text = "     Charger"
        Me.Button_Param.UseVisualStyleBackColor = True
        '
        'RadioButton_Nothing
        '
        Me.RadioButton_Nothing.AutoSize = True
        Me.RadioButton_Nothing.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.RadioButton_Nothing.Location = New System.Drawing.Point(6, 21)
        Me.RadioButton_Nothing.Name = "RadioButton_Nothing"
        Me.RadioButton_Nothing.Size = New System.Drawing.Size(131, 19)
        Me.RadioButton_Nothing.TabIndex = 13
        Me.RadioButton_Nothing.TabStop = True
        Me.RadioButton_Nothing.Text = "Aucun changement"
        Me.RadioButton_Nothing.UseVisualStyleBackColor = True
        '
        'RadioButton_UsersAssets
        '
        Me.RadioButton_UsersAssets.AutoSize = True
        Me.RadioButton_UsersAssets.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.RadioButton_UsersAssets.Location = New System.Drawing.Point(6, 96)
        Me.RadioButton_UsersAssets.Name = "RadioButton_UsersAssets"
        Me.RadioButton_UsersAssets.Size = New System.Drawing.Size(117, 19)
        Me.RadioButton_UsersAssets.TabIndex = 12
        Me.RadioButton_UsersAssets.TabStop = True
        Me.RadioButton_UsersAssets.Text = "Utilisateurs actifs"
        Me.RadioButton_UsersAssets.UseVisualStyleBackColor = True
        '
        'RadioButton_AllCompany
        '
        Me.RadioButton_AllCompany.AutoSize = True
        Me.RadioButton_AllCompany.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.RadioButton_AllCompany.Location = New System.Drawing.Point(6, 71)
        Me.RadioButton_AllCompany.Name = "RadioButton_AllCompany"
        Me.RadioButton_AllCompany.Size = New System.Drawing.Size(133, 19)
        Me.RadioButton_AllCompany.TabIndex = 11
        Me.RadioButton_AllCompany.TabStop = True
        Me.RadioButton_AllCompany.Text = "Tous les utilisateurs"
        Me.RadioButton_AllCompany.UseVisualStyleBackColor = True
        '
        'RadioButton_FolderEDI
        '
        Me.RadioButton_FolderEDI.AutoSize = True
        Me.RadioButton_FolderEDI.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.RadioButton_FolderEDI.Location = New System.Drawing.Point(6, 46)
        Me.RadioButton_FolderEDI.Name = "RadioButton_FolderEDI"
        Me.RadioButton_FolderEDI.Size = New System.Drawing.Size(121, 19)
        Me.RadioButton_FolderEDI.TabIndex = 10
        Me.RadioButton_FolderEDI.TabStop = True
        Me.RadioButton_FolderEDI.Text = "Dossier réception"
        Me.RadioButton_FolderEDI.UseVisualStyleBackColor = True
        '
        'CheckedListBox_UsersEDI
        '
        Me.CheckedListBox_UsersEDI.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CheckedListBox_UsersEDI.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.CheckedListBox_UsersEDI.CheckOnClick = True
        Me.CheckedListBox_UsersEDI.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CheckedListBox_UsersEDI.FormattingEnabled = True
        Me.CheckedListBox_UsersEDI.IntegralHeight = False
        Me.CheckedListBox_UsersEDI.Location = New System.Drawing.Point(12, 30)
        Me.CheckedListBox_UsersEDI.Name = "CheckedListBox_UsersEDI"
        Me.CheckedListBox_UsersEDI.Size = New System.Drawing.Size(218, 292)
        Me.CheckedListBox_UsersEDI.TabIndex = 11
        Me.CheckedListBox_UsersEDI.Visible = False
        '
        'Button_Cancel
        '
        Me.Button_Cancel.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Cancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button_Cancel.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Cancel.Image = Global.MultiUserEDI.My.Resources.Resources.fermer_gtk_icone_6139_32
        Me.Button_Cancel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Cancel.Location = New System.Drawing.Point(236, 84)
        Me.Button_Cancel.Name = "Button_Cancel"
        Me.Button_Cancel.Size = New System.Drawing.Size(139, 40)
        Me.Button_Cancel.TabIndex = 13
        Me.Button_Cancel.Text = "      &Annuler"
        Me.Button_Cancel.UseVisualStyleBackColor = True
        Me.Button_Cancel.Visible = False
        '
        'Button_Valid
        '
        Me.Button_Valid.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Valid.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button_Valid.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Valid.Image = Global.MultiUserEDI.My.Resources.Resources.appliquer_verifier_ok_oui_icone_5318_32
        Me.Button_Valid.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Valid.Location = New System.Drawing.Point(236, 38)
        Me.Button_Valid.Name = "Button_Valid"
        Me.Button_Valid.Size = New System.Drawing.Size(139, 40)
        Me.Button_Valid.TabIndex = 12
        Me.Button_Valid.Text = "      &Valider"
        Me.Button_Valid.UseVisualStyleBackColor = True
        Me.Button_Valid.Visible = False
        '
        'Button_Quitter
        '
        Me.Button_Quitter.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Quitter.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button_Quitter.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Quitter.Image = CType(resources.GetObject("Button_Quitter.Image"), System.Drawing.Image)
        Me.Button_Quitter.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Quitter.Location = New System.Drawing.Point(236, 84)
        Me.Button_Quitter.Name = "Button_Quitter"
        Me.Button_Quitter.Size = New System.Drawing.Size(139, 40)
        Me.Button_Quitter.TabIndex = 2
        Me.Button_Quitter.Text = "     &Quitter"
        Me.Button_Quitter.UseVisualStyleBackColor = True
        '
        'Button_Launch
        '
        Me.Button_Launch.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Button_Launch.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button_Launch.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Launch.Image = Global.MultiUserEDI.My.Resources.Resources.jouer_a_droite_fleche_icone_6822_32
        Me.Button_Launch.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Launch.Location = New System.Drawing.Point(236, 38)
        Me.Button_Launch.Name = "Button_Launch"
        Me.Button_Launch.Size = New System.Drawing.Size(139, 40)
        Me.Button_Launch.TabIndex = 1
        Me.Button_Launch.Text = "     &Exécuter"
        Me.Button_Launch.UseVisualStyleBackColor = True
        '
        'Label_Version
        '
        Me.Label_Version.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label_Version.AutoSize = True
        Me.Label_Version.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Version.Location = New System.Drawing.Point(348, 9)
        Me.Label_Version.Name = "Label_Version"
        Me.Label_Version.Size = New System.Drawing.Size(27, 9)
        Me.Label_Version.TabIndex = 18
        Me.Label_Version.Text = "1.0.0.0"
        '
        'MultiUserEDI
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(387, 334)
        Me.Controls.Add(Me.Label_Version)
        Me.Controls.Add(Me.Button_Cancel)
        Me.Controls.Add(Me.Button_Valid)
        Me.Controls.Add(Me.CheckedListBox_UsersEDI)
        Me.Controls.Add(Me.GroupBoxEDI)
        Me.Controls.Add(Me.LabelVeuillez)
        Me.Controls.Add(Me.Button_Quitter)
        Me.Controls.Add(Me.Button_Launch)
        Me.Controls.Add(Me.ListUsers)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MinimumSize = New System.Drawing.Size(400, 200)
        Me.Name = "MultiUserEDI"
        Me.Text = "Atelier Radio"
        Me.GroupBoxEDI.ResumeLayout(False)
        Me.GroupBoxEDI.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents ListUsers As System.Windows.Forms.ListBox
    Friend WithEvents Button_Launch As System.Windows.Forms.Button
    Friend WithEvents Button_Quitter As System.Windows.Forms.Button
    Friend WithEvents LabelVeuillez As System.Windows.Forms.Label
    Friend WithEvents GroupBoxEDI As System.Windows.Forms.GroupBox
    Friend WithEvents RadioButton_UsersAssets As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton_AllCompany As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton_FolderEDI As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton_Nothing As System.Windows.Forms.RadioButton
    Friend WithEvents CheckedListBox_UsersEDI As System.Windows.Forms.CheckedListBox
    Friend WithEvents Button_Valid As System.Windows.Forms.Button
    Friend WithEvents Button_Cancel As System.Windows.Forms.Button
    Friend WithEvents Button_Param As System.Windows.Forms.Button
    Friend WithEvents RadioButton_AssetsEDI As System.Windows.Forms.RadioButton
    Friend WithEvents Label_Version As System.Windows.Forms.Label

End Class

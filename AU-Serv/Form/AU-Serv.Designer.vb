<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class AUService
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(AUService))
        Me.ProgressBar1 = New System.Windows.Forms.ProgressBar()
        Me.ProgressBar2 = New System.Windows.Forms.ProgressBar()
        Me.Label_Info_Maj = New System.Windows.Forms.Label()
        Me.Label_Etat_Maj = New System.Windows.Forms.Label()
        Me.Label_Etat_Download = New System.Windows.Forms.Label()
        Me.Label_Information = New System.Windows.Forms.Label()
        Me.Label_Percent_Maj = New System.Windows.Forms.Label()
        Me.Label_Name_Maj = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.Button_Suivant = New System.Windows.Forms.Button()
        Me.Button_Fermer = New System.Windows.Forms.Button()
        Me.Button_Execute = New System.Windows.Forms.Button()
        Me.WaitPicture = New System.Windows.Forms.PictureBox()
        Me.Label_Version = New System.Windows.Forms.Label()
        Me.PictureBoxAbout = New System.Windows.Forms.PictureBox()
        Me.PictureSettings = New System.Windows.Forms.PictureBox()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.WaitPicture, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBoxAbout, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureSettings, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'ProgressBar1
        '
        Me.ProgressBar1.Location = New System.Drawing.Point(12, 307)
        Me.ProgressBar1.Name = "ProgressBar1"
        Me.ProgressBar1.Size = New System.Drawing.Size(450, 13)
        Me.ProgressBar1.TabIndex = 0
        '
        'ProgressBar2
        '
        Me.ProgressBar2.Location = New System.Drawing.Point(12, 288)
        Me.ProgressBar2.Name = "ProgressBar2"
        Me.ProgressBar2.Size = New System.Drawing.Size(450, 13)
        Me.ProgressBar2.TabIndex = 1
        '
        'Label_Info_Maj
        '
        Me.Label_Info_Maj.AutoSize = True
        Me.Label_Info_Maj.Font = New System.Drawing.Font("Tahoma", 15.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Info_Maj.Location = New System.Drawing.Point(12, 12)
        Me.Label_Info_Maj.Margin = New System.Windows.Forms.Padding(3)
        Me.Label_Info_Maj.Name = "Label_Info_Maj"
        Me.Label_Info_Maj.Size = New System.Drawing.Size(128, 25)
        Me.Label_Info_Maj.TabIndex = 0
        Me.Label_Info_Maj.Text = "Mise à jour"
        '
        'Label_Etat_Maj
        '
        Me.Label_Etat_Maj.AutoSize = True
        Me.Label_Etat_Maj.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Etat_Maj.Location = New System.Drawing.Point(12, 248)
        Me.Label_Etat_Maj.Margin = New System.Windows.Forms.Padding(3)
        Me.Label_Etat_Maj.Name = "Label_Etat_Maj"
        Me.Label_Etat_Maj.Size = New System.Drawing.Size(116, 14)
        Me.Label_Etat_Maj.TabIndex = 0
        Me.Label_Etat_Maj.Text = "Téléchargement..."
        '
        'Label_Etat_Download
        '
        Me.Label_Etat_Download.AutoSize = True
        Me.Label_Etat_Download.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Etat_Download.Location = New System.Drawing.Point(12, 268)
        Me.Label_Etat_Download.Margin = New System.Windows.Forms.Padding(3)
        Me.Label_Etat_Download.Name = "Label_Etat_Download"
        Me.Label_Etat_Download.Size = New System.Drawing.Size(244, 14)
        Me.Label_Etat_Download.TabIndex = 0
        Me.Label_Etat_Download.Text = "Progression totale du téléchargement:"
        '
        'Label_Information
        '
        Me.Label_Information.AutoSize = True
        Me.Label_Information.Location = New System.Drawing.Point(12, 43)
        Me.Label_Information.Margin = New System.Windows.Forms.Padding(3)
        Me.Label_Information.Name = "Label_Information"
        Me.Label_Information.Size = New System.Drawing.Size(357, 13)
        Me.Label_Information.TabIndex = 0
        Me.Label_Information.Text = "Les éléments suivants vont être téléchargés et installés sur cet ordinateur :"
        '
        'Label_Percent_Maj
        '
        Me.Label_Percent_Maj.AutoSize = True
        Me.Label_Percent_Maj.Location = New System.Drawing.Point(256, 269)
        Me.Label_Percent_Maj.Margin = New System.Windows.Forms.Padding(0, 3, 3, 3)
        Me.Label_Percent_Maj.Name = "Label_Percent_Maj"
        Me.Label_Percent_Maj.Size = New System.Drawing.Size(135, 13)
        Me.Label_Percent_Maj.TabIndex = 0
        Me.Label_Percent_Maj.Text = "Download: 0ko to 0ko (0%)"
        '
        'Label_Name_Maj
        '
        Me.Label_Name_Maj.AutoSize = True
        Me.Label_Name_Maj.Location = New System.Drawing.Point(131, 249)
        Me.Label_Name_Maj.Margin = New System.Windows.Forms.Padding(0, 3, 3, 3)
        Me.Label_Name_Maj.Name = "Label_Name_Maj"
        Me.Label_Name_Maj.Size = New System.Drawing.Size(79, 13)
        Me.Label_Name_Maj.TabIndex = 0
        Me.Label_Name_Maj.Text = "Application.exe"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.AUServ.My.Resources.Resources.kantard
        Me.PictureBox1.Location = New System.Drawing.Point(12, 326)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(84, 36)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.PictureBox1.TabIndex = 13
        Me.PictureBox1.TabStop = False
        '
        'Button_Suivant
        '
        Me.Button_Suivant.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Button_Suivant.Enabled = False
        Me.Button_Suivant.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Suivant.Image = CType(resources.GetObject("Button_Suivant.Image"), System.Drawing.Image)
        Me.Button_Suivant.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Suivant.Location = New System.Drawing.Point(224, 326)
        Me.Button_Suivant.Name = "Button_Suivant"
        Me.Button_Suivant.Size = New System.Drawing.Size(116, 36)
        Me.Button_Suivant.TabIndex = 1
        Me.Button_Suivant.Text = "Mise à jour"
        Me.Button_Suivant.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.Button_Suivant.UseVisualStyleBackColor = True
        '
        'Button_Fermer
        '
        Me.Button_Fermer.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Button_Fermer.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Fermer.Image = CType(resources.GetObject("Button_Fermer.Image"), System.Drawing.Image)
        Me.Button_Fermer.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Fermer.Location = New System.Drawing.Point(346, 326)
        Me.Button_Fermer.Name = "Button_Fermer"
        Me.Button_Fermer.Size = New System.Drawing.Size(116, 36)
        Me.Button_Fermer.TabIndex = 2
        Me.Button_Fermer.Text = "Fermer   "
        Me.Button_Fermer.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.Button_Fermer.UseVisualStyleBackColor = True
        '
        'Button_Execute
        '
        Me.Button_Execute.Enabled = False
        Me.Button_Execute.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button_Execute.Image = CType(resources.GetObject("Button_Execute.Image"), System.Drawing.Image)
        Me.Button_Execute.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.Button_Execute.Location = New System.Drawing.Point(102, 326)
        Me.Button_Execute.Name = "Button_Execute"
        Me.Button_Execute.Size = New System.Drawing.Size(116, 36)
        Me.Button_Execute.TabIndex = 15
        Me.Button_Execute.Text = "Lancer     l'application"
        Me.Button_Execute.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        Me.Button_Execute.UseVisualStyleBackColor = True
        Me.Button_Execute.Visible = False
        '
        'WaitPicture
        '
        Me.WaitPicture.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.WaitPicture.Image = Global.AUServ.My.Resources.Resources.patienter
        Me.WaitPicture.Location = New System.Drawing.Point(12, 62)
        Me.WaitPicture.Name = "WaitPicture"
        Me.WaitPicture.Size = New System.Drawing.Size(450, 180)
        Me.WaitPicture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage
        Me.WaitPicture.TabIndex = 16
        Me.WaitPicture.TabStop = False
        '
        'Label_Version
        '
        Me.Label_Version.AutoSize = True
        Me.Label_Version.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_Version.Location = New System.Drawing.Point(69, 356)
        Me.Label_Version.Name = "Label_Version"
        Me.Label_Version.Size = New System.Drawing.Size(27, 9)
        Me.Label_Version.TabIndex = 17
        Me.Label_Version.Text = "1.0.0.0"
        '
        'PictureBoxAbout
        '
        Me.PictureBoxAbout.Image = Global.AUServ.My.Resources.Resources.About
        Me.PictureBoxAbout.Location = New System.Drawing.Point(438, 12)
        Me.PictureBoxAbout.Name = "PictureBoxAbout"
        Me.PictureBoxAbout.Size = New System.Drawing.Size(24, 24)
        Me.PictureBoxAbout.TabIndex = 19
        Me.PictureBoxAbout.TabStop = False
        '
        'PictureSettings
        '
        Me.PictureSettings.Image = Global.AUServ.My.Resources.Resources.Tools_Settings
        Me.PictureSettings.Location = New System.Drawing.Point(408, 12)
        Me.PictureSettings.Name = "PictureSettings"
        Me.PictureSettings.Size = New System.Drawing.Size(24, 24)
        Me.PictureSettings.TabIndex = 20
        Me.PictureSettings.TabStop = False
        '
        'AUService
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.ClientSize = New System.Drawing.Size(474, 374)
        Me.Controls.Add(Me.PictureSettings)
        Me.Controls.Add(Me.PictureBoxAbout)
        Me.Controls.Add(Me.Label_Version)
        Me.Controls.Add(Me.WaitPicture)
        Me.Controls.Add(Me.Button_Execute)
        Me.Controls.Add(Me.PictureBox1)
        Me.Controls.Add(Me.Label_Name_Maj)
        Me.Controls.Add(Me.Label_Percent_Maj)
        Me.Controls.Add(Me.Label_Information)
        Me.Controls.Add(Me.Button_Suivant)
        Me.Controls.Add(Me.Button_Fermer)
        Me.Controls.Add(Me.Label_Etat_Download)
        Me.Controls.Add(Me.Label_Etat_Maj)
        Me.Controls.Add(Me.Label_Info_Maj)
        Me.Controls.Add(Me.ProgressBar2)
        Me.Controls.Add(Me.ProgressBar1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.Name = "AUService"
        Me.Text = "Auto Update Service"
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.WaitPicture, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBoxAbout, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureSettings, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents ProgressBar1 As System.Windows.Forms.ProgressBar
    Friend WithEvents ProgressBar2 As System.Windows.Forms.ProgressBar
    Friend WithEvents Label_Info_Maj As System.Windows.Forms.Label
    Friend WithEvents Label_Etat_Maj As System.Windows.Forms.Label
    Friend WithEvents Label_Etat_Download As System.Windows.Forms.Label
    Friend WithEvents Button_Fermer As System.Windows.Forms.Button
    Friend WithEvents Button_Suivant As System.Windows.Forms.Button
    Friend WithEvents Label_Information As System.Windows.Forms.Label
    Friend WithEvents Label_Percent_Maj As System.Windows.Forms.Label
    Friend WithEvents Label_Name_Maj As System.Windows.Forms.Label
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents Button_Execute As System.Windows.Forms.Button
    Friend WithEvents WaitPicture As System.Windows.Forms.PictureBox
    Friend WithEvents Label_Version As System.Windows.Forms.Label
    Friend WithEvents PictureBoxAbout As System.Windows.Forms.PictureBox
    Friend WithEvents PictureSettings As System.Windows.Forms.PictureBox

End Class

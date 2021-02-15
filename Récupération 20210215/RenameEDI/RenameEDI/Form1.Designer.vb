<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FormBox
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
        Me.components = New System.ComponentModel.Container()
        Me.FolderBrowserDialog1 = New System.Windows.Forms.FolderBrowserDialog()
        Me.SplitContainer1 = New System.Windows.Forms.SplitContainer()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ComboEmail = New System.Windows.Forms.ComboBox()
        Me.TextPath = New System.Windows.Forms.TextBox()
        Me.TreeViewEDI = New System.Windows.Forms.TreeView()
        Me.CMS_TreeViewEDI = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.TextEDI = New System.Windows.Forms.TextBox()
        Me.DataGridView1 = New System.Windows.Forms.DataGridView()
        Me.MenuStripFM = New System.Windows.Forms.MenuStrip()
        Me.Bt_Users = New System.Windows.Forms.Button()
        Me.Bt_Browse = New System.Windows.Forms.Button()
        Me.SplitContainer1.Panel1.SuspendLayout()
        Me.SplitContainer1.Panel2.SuspendLayout()
        Me.SplitContainer1.SuspendLayout()
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'SplitContainer1
        '
        Me.SplitContainer1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                    Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.SplitContainer1.Location = New System.Drawing.Point(12, 27)
        Me.SplitContainer1.Name = "SplitContainer1"
        Me.SplitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal
        '
        'SplitContainer1.Panel1
        '
        Me.SplitContainer1.Panel1.Controls.Add(Me.Bt_Users)
        Me.SplitContainer1.Panel1.Controls.Add(Me.Label2)
        Me.SplitContainer1.Panel1.Controls.Add(Me.Label1)
        Me.SplitContainer1.Panel1.Controls.Add(Me.ComboEmail)
        Me.SplitContainer1.Panel1.Controls.Add(Me.TextPath)
        Me.SplitContainer1.Panel1.Controls.Add(Me.Bt_Browse)
        Me.SplitContainer1.Panel1.Controls.Add(Me.TreeViewEDI)
        Me.SplitContainer1.Panel1MinSize = 160
        '
        'SplitContainer1.Panel2
        '
        Me.SplitContainer1.Panel2.Controls.Add(Me.TextEDI)
        Me.SplitContainer1.Panel2.Controls.Add(Me.DataGridView1)
        Me.SplitContainer1.Size = New System.Drawing.Size(360, 223)
        Me.SplitContainer1.SplitterDistance = 160
        Me.SplitContainer1.TabIndex = 23
        Me.SplitContainer1.TabStop = False
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(3, 6)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(107, 16)
        Me.Label2.TabIndex = 35
        Me.Label2.Text = "Chemin des EDI:"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(3, 32)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(87, 16)
        Me.Label1.TabIndex = 34
        Me.Label1.Text = "Filtre courriel:"
        '
        'ComboEmail
        '
        Me.ComboEmail.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ComboEmail.FormattingEnabled = True
        Me.ComboEmail.Location = New System.Drawing.Point(96, 31)
        Me.ComboEmail.Name = "ComboEmail"
        Me.ComboEmail.Size = New System.Drawing.Size(258, 21)
        Me.ComboEmail.TabIndex = 1
        '
        'TextPath
        '
        Me.TextPath.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TextPath.Location = New System.Drawing.Point(116, 5)
        Me.TextPath.Name = "TextPath"
        Me.TextPath.ReadOnly = True
        Me.TextPath.Size = New System.Drawing.Size(185, 20)
        Me.TextPath.TabIndex = 32
        Me.TextPath.TabStop = False
        '
        'TreeViewEDI
        '
        Me.TreeViewEDI.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                    Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TreeViewEDI.ContextMenuStrip = Me.CMS_TreeViewEDI
        Me.TreeViewEDI.Location = New System.Drawing.Point(3, 58)
        Me.TreeViewEDI.Name = "TreeViewEDI"
        Me.TreeViewEDI.Size = New System.Drawing.Size(354, 99)
        Me.TreeViewEDI.TabIndex = 2
        Me.TreeViewEDI.TabStop = False
        '
        'CMS_TreeViewEDI
        '
        Me.CMS_TreeViewEDI.Name = "ContextMenuStrip1"
        Me.CMS_TreeViewEDI.Size = New System.Drawing.Size(61, 4)
        '
        'TextEDI
        '
        Me.TextEDI.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                    Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TextEDI.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextEDI.Location = New System.Drawing.Point(3, 2)
        Me.TextEDI.Multiline = True
        Me.TextEDI.Name = "TextEDI"
        Me.TextEDI.ReadOnly = True
        Me.TextEDI.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.TextEDI.Size = New System.Drawing.Size(354, 54)
        Me.TextEDI.TabIndex = 21
        Me.TextEDI.TabStop = False
        Me.TextEDI.Visible = False
        Me.TextEDI.WordWrap = False
        '
        'DataGridView1
        '
        Me.DataGridView1.AllowUserToAddRows = False
        Me.DataGridView1.AllowUserToDeleteRows = False
        Me.DataGridView1.AllowUserToResizeRows = False
        Me.DataGridView1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                    Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells
        Me.DataGridView1.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells
        Me.DataGridView1.Location = New System.Drawing.Point(3, 2)
        Me.DataGridView1.Name = "DataGridView1"
        Me.DataGridView1.ReadOnly = True
        Me.DataGridView1.Size = New System.Drawing.Size(351, 54)
        Me.DataGridView1.TabIndex = 4
        Me.DataGridView1.TabStop = False
        '
        'MenuStripFM
        '
        Me.MenuStripFM.Location = New System.Drawing.Point(0, 0)
        Me.MenuStripFM.Name = "MenuStripFM"
        Me.MenuStripFM.Size = New System.Drawing.Size(384, 24)
        Me.MenuStripFM.TabIndex = 24
        Me.MenuStripFM.Text = "MenuStrip1"
        '
        'Bt_Users
        '
        Me.Bt_Users.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Bt_Users.AutoSize = True
        Me.Bt_Users.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.Bt_Users.Image = Global.GestionEDI.My.Resources.Resources.dossier_utilisateur16
        Me.Bt_Users.Location = New System.Drawing.Point(307, 3)
        Me.Bt_Users.Name = "Bt_Users"
        Me.Bt_Users.Size = New System.Drawing.Size(22, 22)
        Me.Bt_Users.TabIndex = 36
        Me.Bt_Users.TabStop = False
        Me.Bt_Users.UseVisualStyleBackColor = True
        '
        'Bt_Browse
        '
        Me.Bt_Browse.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Bt_Browse.AutoSize = True
        Me.Bt_Browse.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.Bt_Browse.Image = Global.GestionEDI.My.Resources.Resources.explorer16
        Me.Bt_Browse.Location = New System.Drawing.Point(335, 3)
        Me.Bt_Browse.Name = "Bt_Browse"
        Me.Bt_Browse.Size = New System.Drawing.Size(22, 22)
        Me.Bt_Browse.TabIndex = 3
        Me.Bt_Browse.TabStop = False
        Me.Bt_Browse.UseVisualStyleBackColor = True
        '
        'FormBox
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(384, 262)
        Me.Controls.Add(Me.MenuStripFM)
        Me.Controls.Add(Me.SplitContainer1)
        Me.MainMenuStrip = Me.MenuStripFM
        Me.MinimumSize = New System.Drawing.Size(400, 300)
        Me.Name = "FormBox"
        Me.Text = "Gestion des EDI"
        Me.SplitContainer1.Panel1.ResumeLayout(False)
        Me.SplitContainer1.Panel1.PerformLayout()
        Me.SplitContainer1.Panel2.ResumeLayout(False)
        Me.SplitContainer1.Panel2.PerformLayout()
        Me.SplitContainer1.ResumeLayout(False)
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents FolderBrowserDialog1 As System.Windows.Forms.FolderBrowserDialog
    Friend WithEvents SplitContainer1 As System.Windows.Forms.SplitContainer
    Friend WithEvents TextEDI As System.Windows.Forms.TextBox
    Friend WithEvents DataGridView1 As System.Windows.Forms.DataGridView
    Friend WithEvents TreeViewEDI As System.Windows.Forms.TreeView
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Private WithEvents ComboEmail As System.Windows.Forms.ComboBox
    Friend WithEvents TextPath As System.Windows.Forms.TextBox
    Friend WithEvents Bt_Browse As System.Windows.Forms.Button
    Friend WithEvents CMS_TreeViewEDI As System.Windows.Forms.ContextMenuStrip
    Friend WithEvents MenuStripFM As System.Windows.Forms.MenuStrip
    Friend WithEvents Bt_Users As System.Windows.Forms.Button

End Class

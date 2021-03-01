<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
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
        Me.Start = New System.Windows.Forms.Button()
        Me.LabelStatus = New System.Windows.Forms.Label()
        Me.LabelValueStatus = New System.Windows.Forms.Label()
        Me.Rafraichir = New System.Windows.Forms.Button()
        Me.LabelUser = New System.Windows.Forms.Label()
        Me.LabelValueUser = New System.Windows.Forms.Label()
        Me.LabelDate = New System.Windows.Forms.Label()
        Me.LabelValueDate = New System.Windows.Forms.Label()
        Me.Quitter = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'Start
        '
        Me.Start.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Start.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Start.Location = New System.Drawing.Point(106, 125)
        Me.Start.Name = "Start"
        Me.Start.Size = New System.Drawing.Size(80, 24)
        Me.Start.TabIndex = 0
        Me.Start.Text = "Démarrer"
        Me.Start.UseVisualStyleBackColor = True
        '
        'LabelStatus
        '
        Me.LabelStatus.AutoSize = True
        Me.LabelStatus.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelStatus.Location = New System.Drawing.Point(12, 9)
        Me.LabelStatus.Name = "LabelStatus"
        Me.LabelStatus.Size = New System.Drawing.Size(48, 16)
        Me.LabelStatus.TabIndex = 1
        Me.LabelStatus.Text = "Status:"
        '
        'LabelValueStatus
        '
        Me.LabelValueStatus.AutoSize = True
        Me.LabelValueStatus.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelValueStatus.Location = New System.Drawing.Point(66, 9)
        Me.LabelValueStatus.Name = "LabelValueStatus"
        Me.LabelValueStatus.Size = New System.Drawing.Size(44, 16)
        Me.LabelValueStatus.TabIndex = 2
        Me.LabelValueStatus.Text = "Arrêté"
        '
        'Rafraichir
        '
        Me.Rafraichir.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Rafraichir.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Rafraichir.Location = New System.Drawing.Point(20, 125)
        Me.Rafraichir.Name = "Rafraichir"
        Me.Rafraichir.Size = New System.Drawing.Size(80, 24)
        Me.Rafraichir.TabIndex = 3
        Me.Rafraichir.Text = "Rafraichir"
        Me.Rafraichir.UseVisualStyleBackColor = True
        '
        'LabelUser
        '
        Me.LabelUser.AutoSize = True
        Me.LabelUser.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelUser.Location = New System.Drawing.Point(12, 45)
        Me.LabelUser.Name = "LabelUser"
        Me.LabelUser.Size = New System.Drawing.Size(70, 16)
        Me.LabelUser.TabIndex = 4
        Me.LabelUser.Text = "Utilisateur:"
        '
        'LabelValueUser
        '
        Me.LabelValueUser.AutoSize = True
        Me.LabelValueUser.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelValueUser.Location = New System.Drawing.Point(88, 45)
        Me.LabelValueUser.Name = "LabelValueUser"
        Me.LabelValueUser.Size = New System.Drawing.Size(44, 16)
        Me.LabelValueUser.TabIndex = 5
        Me.LabelValueUser.Text = "Néant"
        '
        'LabelDate
        '
        Me.LabelDate.AutoSize = True
        Me.LabelDate.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelDate.Location = New System.Drawing.Point(12, 81)
        Me.LabelDate.Name = "LabelDate"
        Me.LabelDate.Size = New System.Drawing.Size(40, 16)
        Me.LabelDate.TabIndex = 6
        Me.LabelDate.Text = "Date:"
        '
        'LabelValueDate
        '
        Me.LabelValueDate.AutoSize = True
        Me.LabelValueDate.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelValueDate.Location = New System.Drawing.Point(58, 81)
        Me.LabelValueDate.Name = "LabelValueDate"
        Me.LabelValueDate.Size = New System.Drawing.Size(54, 16)
        Me.LabelValueDate.TabIndex = 7
        Me.LabelValueDate.Text = "Inconnu"
        '
        'Quitter
        '
        Me.Quitter.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Quitter.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Quitter.Location = New System.Drawing.Point(192, 125)
        Me.Quitter.Name = "Quitter"
        Me.Quitter.Size = New System.Drawing.Size(80, 24)
        Me.Quitter.TabIndex = 8
        Me.Quitter.Text = "Quitter"
        Me.Quitter.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(284, 161)
        Me.Controls.Add(Me.Quitter)
        Me.Controls.Add(Me.LabelValueDate)
        Me.Controls.Add(Me.LabelDate)
        Me.Controls.Add(Me.LabelValueUser)
        Me.Controls.Add(Me.LabelUser)
        Me.Controls.Add(Me.Rafraichir)
        Me.Controls.Add(Me.LabelValueStatus)
        Me.Controls.Add(Me.LabelStatus)
        Me.Controls.Add(Me.Start)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.MaximizeBox = False
        Me.Name = "Form1"
        Me.Text = "Status Affinage Auto"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Start As System.Windows.Forms.Button
    Friend WithEvents LabelStatus As System.Windows.Forms.Label
    Friend WithEvents LabelValueStatus As System.Windows.Forms.Label
    Friend WithEvents Rafraichir As System.Windows.Forms.Button
    Friend WithEvents LabelUser As System.Windows.Forms.Label
    Friend WithEvents LabelValueUser As System.Windows.Forms.Label
    Friend WithEvents LabelDate As System.Windows.Forms.Label
    Friend WithEvents LabelValueDate As System.Windows.Forms.Label
    Friend WithEvents Quitter As System.Windows.Forms.Button

End Class

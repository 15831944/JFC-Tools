
using System;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace ReStartAffinageAuto
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
			this.Start = new Button();
			this.LabelStatus = new Label();
			this.LabelValueStatus = new Label();
			this.Rafraichir = new Button();
			this.LabelUser = new Label();
			this.LabelValueUser = new Label();
			this.LabelDate = new Label();
			this.LabelValueDate = new Label();
			this.Quitter = new Button();
			base.SuspendLayout();
			this.Start.Anchor = (AnchorStyles.Bottom | AnchorStyles.Right);
			this.Start.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.Start.Location = new Point(106, 125);
			this.Start.Name = "Start";
			this.Start.Size = new Size(80, 24);
			this.Start.TabIndex = 0;
			this.Start.Text = "Démarrer";
			this.Start.UseVisualStyleBackColor = true;
			this.LabelStatus.AutoSize = true;
			this.LabelStatus.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.LabelStatus.Location = new Point(12, 9);
			this.LabelStatus.Name = "LabelStatus";
			this.LabelStatus.Size = new Size(48, 16);
			this.LabelStatus.TabIndex = 1;
			this.LabelStatus.Text = "Status:";
			this.LabelValueStatus.AutoSize = true;
			this.LabelValueStatus.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.LabelValueStatus.Location = new Point(66, 9);
			this.LabelValueStatus.Name = "LabelValueStatus";
			this.LabelValueStatus.Size = new Size(44, 16);
			this.LabelValueStatus.TabIndex = 2;
			this.LabelValueStatus.Text = "Arrêté";
			this.Rafraichir.Anchor = (AnchorStyles.Bottom | AnchorStyles.Right);
			this.Rafraichir.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.Rafraichir.Location = new Point(20, 125);
			this.Rafraichir.Name = "Rafraichir";
			this.Rafraichir.Size = new Size(80, 24);
			this.Rafraichir.TabIndex = 3;
			this.Rafraichir.Text = "Rafraichir";
			this.Rafraichir.UseVisualStyleBackColor = true;
			this.LabelUser.AutoSize = true;
			this.LabelUser.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.LabelUser.Location = new Point(12, 45);
			this.LabelUser.Name = "LabelUser";
			this.LabelUser.Size = new Size(70, 16);
			this.LabelUser.TabIndex = 4;
			this.LabelUser.Text = "Utilisateur:";
			this.LabelValueUser.AutoSize = true;
			this.LabelValueUser.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.LabelValueUser.Location = new Point(88, 45);
			this.LabelValueUser.Name = "LabelValueUser";
			this.LabelValueUser.Size = new Size(44, 16);
			this.LabelValueUser.TabIndex = 5;
			this.LabelValueUser.Text = "Néant";
			this.LabelDate.AutoSize = true;
			this.LabelDate.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.LabelDate.Location = new Point(12, 81);
			this.LabelDate.Name = "LabelDate";
			this.LabelDate.Size = new Size(40, 16);
			this.LabelDate.TabIndex = 6;
			this.LabelDate.Text = "Date:";
			this.LabelValueDate.AutoSize = true;
			this.LabelValueDate.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.LabelValueDate.Location = new Point(58, 81);
			this.LabelValueDate.Name = "LabelValueDate";
			this.LabelValueDate.Size = new Size(54, 16);
			this.LabelValueDate.TabIndex = 7;
			this.LabelValueDate.Text = "Inconnu";
			this.Quitter.Anchor = (AnchorStyles.Bottom | AnchorStyles.Right);
			this.Quitter.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.Quitter.Location = new Point(192, 125);
			this.Quitter.Name = "Quitter";
			this.Quitter.Size = new Size(80, 24);
			this.Quitter.TabIndex = 8;
			this.Quitter.Text = "Quitter";
			this.Quitter.UseVisualStyleBackColor = true;
			base.AutoScaleDimensions = new SizeF(6f, 13f);
			base.AutoScaleMode = AutoScaleMode.Font;
			base.ClientSize = new Size(284, 161);
			base.Controls.Add(this.Quitter);
			base.Controls.Add(this.LabelValueDate);
			base.Controls.Add(this.LabelDate);
			base.Controls.Add(this.LabelValueUser);
			base.Controls.Add(this.LabelUser);
			base.Controls.Add(this.Rafraichir);
			base.Controls.Add(this.LabelValueStatus);
			base.Controls.Add(this.LabelStatus);
			base.Controls.Add(this.Start);
			base.FormBorderStyle = FormBorderStyle.FixedSingle;
			base.MaximizeBox = false;
			base.Name = "Form1";
			this.Text = "Status Affinage Auto";
			base.ResumeLayout(false);
			base.PerformLayout();

		}

		[AccessedThroughProperty("Start"), CompilerGenerated]
		private Button _Start;
		[AccessedThroughProperty("LabelStatus"), CompilerGenerated]
		private Label _LabelStatus;
		[AccessedThroughProperty("LabelValueStatus"), CompilerGenerated]
		private Label _LabelValueStatus;
		[AccessedThroughProperty("Rafraichir"), CompilerGenerated]
		private Button _Rafraichir;
		[AccessedThroughProperty("LabelUser"), CompilerGenerated]
		private Label _LabelUser;
		[AccessedThroughProperty("LabelValueUser"), CompilerGenerated]
		private Label _LabelValueUser;
		[AccessedThroughProperty("LabelDate"), CompilerGenerated]
		private Label _LabelDate;
		[AccessedThroughProperty("LabelValueDate"), CompilerGenerated]
		private Label _LabelValueDate;
		[AccessedThroughProperty("Quitter"), CompilerGenerated]
		private Button _Quitter;
		internal virtual Button Start
		{
			[CompilerGenerated]
			get
			{
				return this._Start;
			}
			[CompilerGenerated]
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.Start_Click);
				Button start = this._Start;
				if (start != null)
				{
					start.Click -= value2;
				}
				this._Start = value;
				start = this._Start;
				if (start != null)
				{
					start.Click += value2;
				}
			}
		}
		internal virtual Label LabelStatus
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual Label LabelValueStatus
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual Button Rafraichir
		{
			[CompilerGenerated]
			get
			{
				return this._Rafraichir;
			}
			[CompilerGenerated]
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.Rafraichir_Click);
				Button rafraichir = this._Rafraichir;
				if (rafraichir != null)
				{
					rafraichir.Click -= value2;
				}
				this._Rafraichir = value;
				rafraichir = this._Rafraichir;
				if (rafraichir != null)
				{
					rafraichir.Click += value2;
				}
			}
		}
		internal virtual Label LabelUser
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual Label LabelValueUser
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual Label LabelDate
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual Label LabelValueDate
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual Button Quitter
		{
			[CompilerGenerated]
			get
			{
				return this._Quitter;
			}
			[CompilerGenerated]
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.Quitter_Click);
				Button quitter = this._Quitter;
				if (quitter != null)
				{
					quitter.Click -= value2;
				}
				this._Quitter = value;
				quitter = this._Quitter;
				if (quitter != null)
				{
					quitter.Click += value2;
				}
			}
		}


		#endregion

	}
}


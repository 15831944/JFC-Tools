
using System;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace Facility_Management
{
    partial class GetElements
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		[System.Diagnostics.DebuggerStepThrough]
		private void InitializeComponent()
		{
			ComboBoxVersion = new System.Windows.Forms.ComboBox();
			ChooseLabel = new System.Windows.Forms.Label();
			ButtonCancel = new System.Windows.Forms.Button();
			ButtonGet = new System.Windows.Forms.Button();
			ButtonIgnore = new System.Windows.Forms.Button();
			SuspendLayout();
			ComboBoxVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
			ComboBoxVersion.FormattingEnabled = true;
			ComboBoxVersion.Location = new System.Drawing.Point(12, 30);
			ComboBoxVersion.Name = "ComboBoxVersion";
			ComboBoxVersion.Size = new System.Drawing.Size(288, 26);
			ComboBoxVersion.TabIndex = 0;
			ChooseLabel.AutoSize = true;
			ChooseLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
			ChooseLabel.Location = new System.Drawing.Point(12, 9);
			ChooseLabel.Name = "ChooseLabel";
			ChooseLabel.Size = new System.Drawing.Size(102, 18);
			ChooseLabel.TabIndex = 1;
			ChooseLabel.Text = "Get Elements:";
			ButtonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			ButtonCancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
			ButtonCancel.Location = new System.Drawing.Point(110, 62);
			ButtonCancel.Name = "ButtonCancel";
			ButtonCancel.Size = new System.Drawing.Size(92, 30);
			ButtonCancel.TabIndex = 2;
			ButtonCancel.Text = "&Cancel";
			ButtonCancel.UseVisualStyleBackColor = true;
			ButtonGet.DialogResult = System.Windows.Forms.DialogResult.OK;
			ButtonGet.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
			ButtonGet.Location = new System.Drawing.Point(208, 62);
			ButtonGet.Name = "ButtonGet";
			ButtonGet.Size = new System.Drawing.Size(92, 30);
			ButtonGet.TabIndex = 3;
			ButtonGet.Text = "&Get";
			ButtonGet.UseVisualStyleBackColor = true;
			ButtonIgnore.DialogResult = System.Windows.Forms.DialogResult.Ignore;
			ButtonIgnore.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
			ButtonIgnore.Location = new System.Drawing.Point(12, 62);
			ButtonIgnore.Name = "ButtonIgnore";
			ButtonIgnore.Size = new System.Drawing.Size(92, 30);
			ButtonIgnore.TabIndex = 4;
			ButtonIgnore.Text = "&Ignore";
			ButtonIgnore.UseVisualStyleBackColor = true;
			base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
			base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			base.ClientSize = new System.Drawing.Size(312, 104);
			base.Controls.Add(ButtonIgnore);
			base.Controls.Add(ButtonGet);
			base.Controls.Add(ButtonCancel);
			base.Controls.Add(ChooseLabel);
			base.Controls.Add(ComboBoxVersion);
			base.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			base.MaximizeBox = false;
			base.Name = "GetElements";
			base.ShowInTaskbar = false;
			base.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			Text = "Get Elements";
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion

		[CompilerGenerated]
		[AccessedThroughProperty("ComboBoxVersion")]
		private ComboBox _ComboBoxVersion;

		[CompilerGenerated]
		[AccessedThroughProperty("ChooseLabel")]
		private Label _ChooseLabel;

		[CompilerGenerated]
		[AccessedThroughProperty("ButtonCancel")]
		private Button _ButtonCancel;

		[CompilerGenerated]
		[AccessedThroughProperty("ButtonGet")]
		private Button _ButtonGet;

		[CompilerGenerated]
		[AccessedThroughProperty("ButtonIgnore")]
		private Button _ButtonIgnore;

		internal virtual ComboBox ComboBoxVersion
		{
			[CompilerGenerated]
			get
			{
				return _ComboBoxVersion;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			[CompilerGenerated]
			set
			{
				KeyPressEventHandler value2 = ComboBoxVersion_KeyPress;
				ComboBox comboBoxVersion = _ComboBoxVersion;
				if (comboBoxVersion != null)
				{
					comboBoxVersion.KeyPress -= value2;
				}
				_ComboBoxVersion = value;
				comboBoxVersion = _ComboBoxVersion;
				if (comboBoxVersion != null)
				{
					comboBoxVersion.KeyPress += value2;
				}
			}
		}

		[field: AccessedThroughProperty("ChooseLabel")]
		internal virtual Label ChooseLabel
		{
			get; [MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}

		internal virtual Button ButtonCancel
		{
			[CompilerGenerated]
			get
			{
				return _ButtonCancel;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			[CompilerGenerated]
			set
			{
				EventHandler value2 = ButtonCancel_Click;
				Button buttonCancel = _ButtonCancel;
				if (buttonCancel != null)
				{
					buttonCancel.Click -= value2;
				}
				_ButtonCancel = value;
				buttonCancel = _ButtonCancel;
				if (buttonCancel != null)
				{
					buttonCancel.Click += value2;
				}
			}
		}

		internal virtual Button ButtonGet
		{
			[CompilerGenerated]
			get
			{
				return _ButtonGet;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			[CompilerGenerated]
			set
			{
				EventHandler value2 = ButtonGet_Click;
				Button buttonGet = _ButtonGet;
				if (buttonGet != null)
				{
					buttonGet.Click -= value2;
				}
				_ButtonGet = value;
				buttonGet = _ButtonGet;
				if (buttonGet != null)
				{
					buttonGet.Click += value2;
				}
			}
		}

		internal virtual Button ButtonIgnore
		{
			[CompilerGenerated]
			get
			{
				return _ButtonIgnore;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			[CompilerGenerated]
			set
			{
				EventHandler value2 = ButtonIgnore_Click;
				Button buttonIgnore = _ButtonIgnore;
				if (buttonIgnore != null)
				{
					buttonIgnore.Click -= value2;
				}
				_ButtonIgnore = value;
				buttonIgnore = _ButtonIgnore;
				if (buttonIgnore != null)
				{
					buttonIgnore.Click += value2;
				}
			}
		}

	}
}
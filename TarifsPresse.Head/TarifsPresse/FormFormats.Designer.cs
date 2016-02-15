namespace TarifsPresse
{
    partial class FormFormats
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
        private void InitializeComponent()
        {
            this.listViewFormat = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonMapperFormat = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxSelectedFormatCode = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxSelectedFormat = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.buttonCreerFormat = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxFormatToCreate = new System.Windows.Forms.TextBox();
            this.textBoxFormatCodeToCreate = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxFormatToMap = new System.Windows.Forms.TextBox();
            this.buttonIgnoreFormat = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewFormat
            // 
            this.listViewFormat.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.listViewFormat.Location = new System.Drawing.Point(13, 37);
            this.listViewFormat.MultiSelect = false;
            this.listViewFormat.Name = "listViewFormat";
            this.listViewFormat.Size = new System.Drawing.Size(227, 504);
            this.listViewFormat.TabIndex = 0;
            this.listViewFormat.UseCompatibleStateImageBehavior = false;
            this.listViewFormat.View = System.Windows.Forms.View.Details;
            this.listViewFormat.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Formats";
            this.columnHeader1.Width = 222;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(130, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Liste des formats connus :";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonMapperFormat);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBoxSelectedFormatCode);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBoxSelectedFormat);
            this.groupBox1.Location = new System.Drawing.Point(246, 79);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(258, 228);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Format Selectionné";
            // 
            // buttonMapperFormat
            // 
            this.buttonMapperFormat.Enabled = false;
            this.buttonMapperFormat.Location = new System.Drawing.Point(46, 152);
            this.buttonMapperFormat.Name = "buttonMapperFormat";
            this.buttonMapperFormat.Size = new System.Drawing.Size(166, 23);
            this.buttonMapperFormat.TabIndex = 4;
            this.buttonMapperFormat.Text = "Associer ce format";
            this.buttonMapperFormat.UseVisualStyleBackColor = true;
            this.buttonMapperFormat.Click += new System.EventHandler(this.buttonMapperFormat_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 59);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(260, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Le code du format (5 chars format + 5 chars couleur) :";
            // 
            // textBoxSelectedFormatCode
            // 
            this.textBoxSelectedFormatCode.Location = new System.Drawing.Point(6, 91);
            this.textBoxSelectedFormatCode.Name = "textBoxSelectedFormatCode";
            this.textBoxSelectedFormatCode.ReadOnly = true;
            this.textBoxSelectedFormatCode.Size = new System.Drawing.Size(246, 20);
            this.textBoxSelectedFormatCode.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(95, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Le nom du format :";
            // 
            // textBoxSelectedFormat
            // 
            this.textBoxSelectedFormat.Location = new System.Drawing.Point(6, 36);
            this.textBoxSelectedFormat.Name = "textBoxSelectedFormat";
            this.textBoxSelectedFormat.ReadOnly = true;
            this.textBoxSelectedFormat.Size = new System.Drawing.Size(246, 20);
            this.textBoxSelectedFormat.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.buttonCreerFormat);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.textBoxFormatToCreate);
            this.groupBox2.Controls.Add(this.textBoxFormatCodeToCreate);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Location = new System.Drawing.Point(247, 313);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(257, 199);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Nouveau format";
            // 
            // buttonCreerFormat
            // 
            this.buttonCreerFormat.Enabled = false;
            this.buttonCreerFormat.Location = new System.Drawing.Point(45, 137);
            this.buttonCreerFormat.Name = "buttonCreerFormat";
            this.buttonCreerFormat.Size = new System.Drawing.Size(166, 23);
            this.buttonCreerFormat.TabIndex = 9;
            this.buttonCreerFormat.Text = "Créer ce format";
            this.buttonCreerFormat.UseVisualStyleBackColor = true;
            this.buttonCreerFormat.Click += new System.EventHandler(this.buttonCreerFormat_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 63);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(260, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Le code du format (5 chars format + 5 chars couleur) :";
            // 
            // textBoxFormatToCreate
            // 
            this.textBoxFormatToCreate.Location = new System.Drawing.Point(5, 40);
            this.textBoxFormatToCreate.Name = "textBoxFormatToCreate";
            this.textBoxFormatToCreate.Size = new System.Drawing.Size(246, 20);
            this.textBoxFormatToCreate.TabIndex = 5;
            this.textBoxFormatToCreate.TextChanged += new System.EventHandler(this.textBoxFormatToCreate_TextChanged);
            // 
            // textBoxFormatCodeToCreate
            // 
            this.textBoxFormatCodeToCreate.BackColor = System.Drawing.Color.White;
            this.textBoxFormatCodeToCreate.Location = new System.Drawing.Point(5, 79);
            this.textBoxFormatCodeToCreate.MaxLength = 10;
            this.textBoxFormatCodeToCreate.Name = "textBoxFormatCodeToCreate";
            this.textBoxFormatCodeToCreate.Size = new System.Drawing.Size(246, 20);
            this.textBoxFormatCodeToCreate.TabIndex = 7;
            this.textBoxFormatCodeToCreate.TextChanged += new System.EventHandler(this.textBoxFormatCodeToCreate_TextChanged);
            this.textBoxFormatCodeToCreate.KeyUp += new System.Windows.Forms.KeyEventHandler(this.textBoxFormatCodeToCreate_KeyUp);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 24);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(95, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "Le nom du format :";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(253, 37);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(146, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Le nom du format à associer :";
            // 
            // textBoxFormatToMap
            // 
            this.textBoxFormatToMap.Location = new System.Drawing.Point(252, 53);
            this.textBoxFormatToMap.Name = "textBoxFormatToMap";
            this.textBoxFormatToMap.ReadOnly = true;
            this.textBoxFormatToMap.Size = new System.Drawing.Size(252, 20);
            this.textBoxFormatToMap.TabIndex = 5;
            // 
            // buttonIgnoreFormat
            // 
            this.buttonIgnoreFormat.Location = new System.Drawing.Point(292, 518);
            this.buttonIgnoreFormat.Name = "buttonIgnoreFormat";
            this.buttonIgnoreFormat.Size = new System.Drawing.Size(166, 23);
            this.buttonIgnoreFormat.TabIndex = 10;
            this.buttonIgnoreFormat.Text = "Ignorer ce format";
            this.buttonIgnoreFormat.UseVisualStyleBackColor = true;
            this.buttonIgnoreFormat.Click += new System.EventHandler(this.buttonIgnoreFormat_Click);
            // 
            // FormFormats
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(516, 554);
            this.Controls.Add(this.buttonIgnoreFormat);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBoxFormatToMap);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listViewFormat);
            this.Name = "FormFormats";
            this.Text = "Choisir le format correspondant ou en créer un nouveau";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormFormats_FormClosing);
            this.Load += new System.EventHandler(this.FormFormats_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listViewFormat;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonMapperFormat;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxSelectedFormatCode;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxSelectedFormat;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonCreerFormat;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxFormatToCreate;
        private System.Windows.Forms.TextBox textBoxFormatCodeToCreate;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxFormatToMap;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Button buttonIgnoreFormat;
    }
}
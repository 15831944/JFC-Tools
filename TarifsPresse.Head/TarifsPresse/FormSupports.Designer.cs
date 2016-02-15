namespace TarifsPresse
{
    partial class FormSupports
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
            this.listViewSupports = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonMapperSupport = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxSelectedSupportCode = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxSelectedSupport = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.buttonCreerSupport = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxSupportToCreate = new System.Windows.Forms.TextBox();
            this.textBoxSupportCodeToCreate = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxSupportToMap = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewSupports
            // 
            this.listViewSupports.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.listViewSupports.Location = new System.Drawing.Point(13, 37);
            this.listViewSupports.MultiSelect = false;
            this.listViewSupports.Name = "listViewSupports";
            this.listViewSupports.Size = new System.Drawing.Size(227, 504);
            this.listViewSupports.TabIndex = 0;
            this.listViewSupports.UseCompatibleStateImageBehavior = false;
            this.listViewSupports.View = System.Windows.Forms.View.Details;
            this.listViewSupports.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Supports";
            this.columnHeader1.Width = 222;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(136, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Liste des supports connus :";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonMapperSupport);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBoxSelectedSupportCode);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBoxSelectedSupport);
            this.groupBox1.Location = new System.Drawing.Point(246, 79);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(227, 228);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Support Selectionné";
            // 
            // buttonMapperSupport
            // 
            this.buttonMapperSupport.Enabled = false;
            this.buttonMapperSupport.Location = new System.Drawing.Point(30, 152);
            this.buttonMapperSupport.Name = "buttonMapperSupport";
            this.buttonMapperSupport.Size = new System.Drawing.Size(166, 23);
            this.buttonMapperSupport.TabIndex = 4;
            this.buttonMapperSupport.Text = "Mapper ce support";
            this.buttonMapperSupport.UseVisualStyleBackColor = true;
            this.buttonMapperSupport.Click += new System.EventHandler(this.buttonMapperSupport_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 59);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(171, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Le code du support (6 chars max) :";
            // 
            // textBoxSelectedSupportCode
            // 
            this.textBoxSelectedSupportCode.Location = new System.Drawing.Point(6, 75);
            this.textBoxSelectedSupportCode.Name = "textBoxSelectedSupportCode";
            this.textBoxSelectedSupportCode.ReadOnly = true;
            this.textBoxSelectedSupportCode.Size = new System.Drawing.Size(214, 20);
            this.textBoxSelectedSupportCode.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(101, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Le nom du support :";
            // 
            // textBoxSelectedSupport
            // 
            this.textBoxSelectedSupport.Location = new System.Drawing.Point(6, 36);
            this.textBoxSelectedSupport.Name = "textBoxSelectedSupport";
            this.textBoxSelectedSupport.ReadOnly = true;
            this.textBoxSelectedSupport.Size = new System.Drawing.Size(214, 20);
            this.textBoxSelectedSupport.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.buttonCreerSupport);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.textBoxSupportToCreate);
            this.groupBox2.Controls.Add(this.textBoxSupportCodeToCreate);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Location = new System.Drawing.Point(247, 313);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(227, 228);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Nouveau support";
            // 
            // buttonCreerSupport
            // 
            this.buttonCreerSupport.Enabled = false;
            this.buttonCreerSupport.Location = new System.Drawing.Point(29, 156);
            this.buttonCreerSupport.Name = "buttonCreerSupport";
            this.buttonCreerSupport.Size = new System.Drawing.Size(166, 23);
            this.buttonCreerSupport.TabIndex = 9;
            this.buttonCreerSupport.Text = "Créer ce support";
            this.buttonCreerSupport.UseVisualStyleBackColor = true;
            this.buttonCreerSupport.Click += new System.EventHandler(this.buttonCreerSupport_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 63);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(171, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Le code du support (6 chars max) :";
            // 
            // textBoxSupportToCreate
            // 
            this.textBoxSupportToCreate.Location = new System.Drawing.Point(5, 40);
            this.textBoxSupportToCreate.Name = "textBoxSupportToCreate";
            this.textBoxSupportToCreate.Size = new System.Drawing.Size(214, 20);
            this.textBoxSupportToCreate.TabIndex = 5;
            this.textBoxSupportToCreate.TextChanged += new System.EventHandler(this.textBoxSupportToCreate_TextChanged);
            // 
            // textBoxSupportCodeToCreate
            // 
            this.textBoxSupportCodeToCreate.BackColor = System.Drawing.Color.White;
            this.textBoxSupportCodeToCreate.Location = new System.Drawing.Point(5, 79);
            this.textBoxSupportCodeToCreate.MaxLength = 6;
            this.textBoxSupportCodeToCreate.Name = "textBoxSupportCodeToCreate";
            this.textBoxSupportCodeToCreate.Size = new System.Drawing.Size(214, 20);
            this.textBoxSupportCodeToCreate.TabIndex = 7;
            this.textBoxSupportCodeToCreate.TextChanged += new System.EventHandler(this.textBoxSupportCodeToCreate_TextChanged);
            this.textBoxSupportCodeToCreate.KeyUp += new System.Windows.Forms.KeyEventHandler(this.textBoxSupportCodeToCreate_KeyUp);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 24);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(101, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "Le nom du support :";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(253, 37);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(148, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Le nom du support à mapper :";
            // 
            // textBoxSupportToMap
            // 
            this.textBoxSupportToMap.Location = new System.Drawing.Point(252, 53);
            this.textBoxSupportToMap.Name = "textBoxSupportToMap";
            this.textBoxSupportToMap.ReadOnly = true;
            this.textBoxSupportToMap.Size = new System.Drawing.Size(214, 20);
            this.textBoxSupportToMap.TabIndex = 5;
            // 
            // FormSupports
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 554);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBoxSupportToMap);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listViewSupports);
            this.Name = "FormSupports";
            this.Text = "Choisir le support correspondant ou en créer un nouveau";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormSupports_FormClosing);
            this.Load += new System.EventHandler(this.FormSupports_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listViewSupports;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonMapperSupport;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxSelectedSupportCode;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxSelectedSupport;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonCreerSupport;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxSupportToCreate;
        private System.Windows.Forms.TextBox textBoxSupportCodeToCreate;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxSupportToMap;
        private System.Windows.Forms.ColumnHeader columnHeader1;
    }
}
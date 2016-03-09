using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Serialization;

namespace PopCornToAtelierRadio
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            progressBar1.Hide();
            LoadConfig();
            this.UpdateListBox();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {

            FolderBrowserDialog myfolder = new FolderBrowserDialog();
            myfolder.SelectedPath = pathAR.Text;

            myfolder.ShowDialog();

            pathAR.Text = myfolder.SelectedPath;

            m_config.Ar = myfolder.SelectedPath;
            this.SaveConfig();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog myfolder = new FolderBrowserDialog();
            myfolder.SelectedPath = textOut.Text;

            myfolder.ShowDialog();

            textOut.Text = myfolder.SelectedPath;

            m_config.Out = myfolder.SelectedPath;
            this.SaveConfig();


        }

        private void button3_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog myfolder = new FolderBrowserDialog();
            myfolder.SelectedPath = textIn.Text;

            myfolder.ShowDialog();

            textIn.Text = myfolder.SelectedPath;

            this.UpdateListBox();

            m_config.In = myfolder.SelectedPath;
            this.SaveConfig();

        }

        private void button4_Click(object sender, EventArgs e)
        {
            progressBar1.Show();
            progressBar1.Minimum = 0;
            progressBar1.Maximum = listFilePop.SelectedItems.Count;
            progressBar1.Value = 0;

            m_journal.Items.Clear();
            List<JournalLine> l_journal = new List<JournalLine>();
            int nbFileWritten = 0;

            foreach (string st in listFilePop.SelectedItems) 
            {
                PopToRadioTranslator l_translator = new PopToRadioTranslator(textIn.Text + "\\" + st, pathAR.Text, textOut.Text, l_journal);
               if (l_translator.FileWritten)
                   ++nbFileWritten;
                progressBar1.Value += 1; 
            }
            l_journal.Add(new JournalLine("export terminé.. Nombre de fichiers traduits: " + nbFileWritten.ToString(), true));
            progressBar1.Hide();
            TextWriter writd = new StreamWriter("journal.txt", false);

            foreach (JournalLine st in l_journal)
            {
                if (st.m_showLine)
                    m_journal.Items.Add(st.m_line);

                writd.WriteLine(st.m_line);

            }
            writd.Close();
           // PopToRadioTranslator l_translator = new PopToRadioTranslator("GMF Janvier.xml");
        }

        private void LoadConfig()
        {
            if (File.Exists("config.xml"))
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Config));
                StreamReader file = new StreamReader("config.xml");

                m_config =(Config) serializer.Deserialize(file);
                file.Close();

                pathAR.Text = m_config.Ar;
                textIn.Text = m_config.In;
                textOut.Text = m_config.Out;
            }
            else
            {
                m_config = new Config();
            }
            


        }
        private void UpdateListBox()
        {

            if (Directory.Exists(textIn.Text))
            {
                listFilePop.Items.Clear();
                IEnumerable<string> files = Directory.GetFiles(textIn.Text).Where(s => Path.GetExtension(s).Equals(".xml"));


                int index = 0;
                foreach (string file in files)
                {

                    listFilePop.Items.Add(Path.GetFileName(file));
                    listFilePop.SetSelected(index, true);
                    index += 1;
                }

            }
        }

        private void SaveConfig()
        {
            XmlSerializer serializer = new XmlSerializer(m_config.GetType());
            StreamWriter file = new StreamWriter("config.xml");
            serializer.Serialize(file, m_config);

            file.Close();
        }

        private Config m_config;

        private void BtnRazSel_Click(object sender, EventArgs e)
        {
            listFilePop.ClearSelected();
        }
    }
}

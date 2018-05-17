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
using System.IO.Packaging;
using System.Xml;

namespace AR_reconstitution
{
    public partial class Form1 : Form
    {

        StudyBuilder _builder = new StudyBuilder();
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            String _directory = "1802-OP2602";
            String _outName = "1802-OP2602";

            if (Directory.Exists(_directory))
            {
                String l_cartouchePath = _directory + "/cartouche.xml";

                if (File.Exists(l_cartouchePath))
                {
                  /*  StreamReader l_readerCart = new StreamReader(l_cartouchePath);
                    XmlSerializer SerializerCart = new XmlSerializer(typeof(Cartouche));

                    Cartouche eltCartouche = (Cartouche)SerializerCart.Deserialize(l_readerCart);

                    l_readerCart.Close();*/

                    // Création du fichier package
                    String l_outfile = _directory + "/" + _outName;
                    l_outfile += ".etd";
                    /*  switch (eltCartouche.Info.Etape)
                      {
                          case 1:
                              // Cas d'une étude à rezipper
                              l_outfile += ".etd";
                              break;

                          case 2:
                              // cas d'une campagne à rezipper
                              l_outfile += ".cmp";
                              break;
                      }*/

                    Package myzipFile = ZipPackage.Open(l_outfile, FileMode.Create);

                    String fileRelation = _directory + @"\_rels\.rels";

                    // Créer stream lecture via le fichier
                    StreamReader l_reader = new StreamReader(fileRelation);

                    // Créer instance sérialisation avec classe attendu
                    XmlSerializer SerializerObj = new XmlSerializer(typeof(Relationships));
                    Relationships eltRlts = (Relationships)SerializerObj.Deserialize(l_reader);

                    // Boucle sur tous les fichiers à zipper
                    foreach (RelationshipsRelationship irlt in eltRlts.Relationship)
                    {
                        // Récup Nom fichier à packager
                        String l_pathFile = Path.Combine(Directory.GetCurrentDirectory(), irlt.Target);
                        StreamReader l_rltReader = new StreamReader(_directory + l_pathFile);

                        Uri zipPartUri = PackUriHelper.CreatePartUri(new Uri(irlt.Target, UriKind.Relative));

                        // <création partition config
                        PackagePart configsPart = myzipFile.CreatePart(zipPartUri, System.Net.Mime.MediaTypeNames.Text.Xml, CompressionOption.Normal);
                        StreamWriter streamWriter = new StreamWriter(configsPart.GetStream(FileMode.Create));

                        streamWriter.Write(l_rltReader.ReadToEnd());
                        streamWriter.Close();
                        l_rltReader.Close();

                        myzipFile.CreateRelationship(configsPart.Uri, TargetMode.Internal, irlt.Type, irlt.Id);


                    }

                    // Fermeture Reader relations
                    l_reader.Close();

                    // Fermeture du zip file
                    myzipFile.Close();

                    // Fin packaging
   
                }


            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        public string GetNameRelationShip(string Name)
        {
            return System.Convert.ToBase64String(System.Text.Encoding.UTF8.GetBytes(Name)).Replace('+', '-').Replace('/', '_').Trim('=');
        }

        public string GetNameFilePlanXml(string NamePlan, ulong NoPlan, short Etat)
        {
            short ShortNoPlan = (short)NoPlan;   // Pour garder le format utilisé auparavant
            if (NoPlan < 100000)
            {
                // Ici il s'agit d'un plan étude ou campagne classique
                string NameFileXml = "T" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan.Replace('/', '-').Replace(':', '_') + ".xml";
                return NameFileXml;
            }
            else
            {
                // Ici il s'agit d'un plan echange EDI
                // string NameFileXml = "EDI" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan.Replace('/', '-').Replace(':', '_') + ".xml";

                // Pour le moment on ne change pas le nom du fichier  (seule la relationship "Type" sera différente)
                string NameFileXml = "T" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan.Replace('/', '-').Replace(':', '_') + ".xml";
                return NameFileXml;
            }
        }


        private void button2_Click(object sender, EventArgs e)
        {


            Dictionary<String, XmlSerializer> l_serializers =  new Dictionary<string, XmlSerializer>();
            l_serializers.Add("cartouche", new XmlSerializer(typeof(Cartouche)));
            l_serializers.Add("plan", new XmlSerializer(typeof(PlansPlan)));

            l_serializers.Add("composteur", new XmlSerializer(typeof(Composteurs)));
            l_serializers.Add("substa", new XmlSerializer(typeof(SubStas)));

            File.Copy("base.etd", "base_2.etd", true);


            using (Package l_studyO = ZipPackage.Open("base.etd"))
            using (Package l_studyS = ZipPackage.Open("base_2.etd"))
            {
                PackagePartCollection l_refPackage = l_studyO.GetParts();

                foreach (PackagePart i_part in l_refPackage)
                {
                    object l_deserialized = null;
                    String l_currentSerialier = "";

                    if (i_part.Uri.OriginalString.Contains("history"))
                    {
                        continue;
                    }


                    foreach (KeyValuePair<String, XmlSerializer> i_serializer in l_serializers)
                    {

                        try
                        {
                            l_deserialized = i_serializer.Value.Deserialize(i_part.GetStream());
                            if (l_deserialized != null)
                            {
                                l_currentSerialier = i_serializer.Key;
                                break;
                            }
                        }
                        catch
                        {

                        }
                    }
                    if (l_deserialized != null)
                    {
                        switch (l_currentSerialier)
                        {
                            case "cartouche":

                                l_studyS.CreateRelationship(i_part.Uri, TargetMode.Internal, "cartouche", "cartouche");
                                break;

                            case "composteur":
                                 l_studyS.CreateRelationship(i_part.Uri, TargetMode.Internal, "composteur", "composteur");
                                break;
                            case "substa":
                                l_studyS.CreateRelationship(i_part.Uri, TargetMode.Internal, "substation", "substation");
                                break;
                           

                            case "plan":

                                PlansPlan l_plan = l_deserialized as PlansPlan;
                                if (l_plan == null)
                                    continue;

                                // JUILLET 2015 Lecture des plans selon type
                               

                                // Virer le plan N° 0 si déjà existant (cas d'ancienne campagne sauvée avec nom réel du plan / pour éviter doublon)
                                
                         
                                // Création nom de l'élément (Type Occupation sur 1 caractère + nom du plan)
                                string NamePlanState = GetNameFilePlanXml(l_plan.Nom, l_plan.NrPlan, l_plan.Occupe);

                                // Nom pour RelationShip (convertit tous les caracteres interdits Xml , juste pour la relationship)
                                string NameRelationShip = GetNameRelationShip(NamePlanState);



                                l_studyS.CreateRelationship(i_part.Uri, TargetMode.Internal, "plan", NameRelationShip);
                                break;
                        }



                    }

                }
                l_studyO.Close();
                l_studyS.Close();


            }
            MessageBox.Show("done");

        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (String.IsNullOrEmpty(this.textBoxFilename.Text))
            {
                 MessageBox.Show("entrer le nom du fichier xml");
                return;
            }
           String lrlt = System.Convert.ToBase64String(System.Text.Encoding.UTF8.GetBytes(this.textBoxFilename.Text)).Replace('+', '1')
                        .Replace('/', '2')
                        .Replace('\\', '2')
                        .Replace('%', '3')
                        .Replace('[', '4')
                        .Replace(']', '5')
                        .Replace('&', '6')
                        .Replace('#', '7')
                        .Replace('|', '8')
                        .Replace('@', '9')
                        .Replace('_', '1')
                        .Replace('"', '2')
                        .Replace('\'', '3')
                        .Replace('{', '4')
                        .Replace('}', '5')
                        .Replace('`', '6')
                        .Replace('^', '7')
                        .Trim('=');

            this.textBoxRLT.Text = lrlt;
        }
        
    
        private void textBoxFilename_TextChanged(object sender, EventArgs e)
        {

        }

        private void buildDirectory_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog ldialog = new FolderBrowserDialog();
      ;
            ldialog.ShowNewFolderButton = false;
            ldialog.SelectedPath = Environment.CurrentDirectory;


            DialogResult lResult = ldialog.ShowDialog();
            if (lResult == DialogResult.OK)
            {
                bool lSuccess = _builder.BuildFromDirectory(ldialog.SelectedPath);
                String lmessage = "operation reussie";
                if ( ! lSuccess)
                {
                    lmessage = "Une erreur est survenue : " + _builder.Journal.LastOrDefault() +Environment.NewLine +" Pour plus de détails lire le fichier.\\logs.out";
                   
                }
                this.WriteLog(_builder.Journal);
                MessageBox.Show(lmessage);
            }
        }



        private void WriteLog(List<String> journal)
        {
            if (journal == null)
                return;

            using (StreamWriter lwriter = new StreamWriter("log.out",true))
            {
                foreach (string s in journal)
                {
                    lwriter.WriteLine("{0} : {1}", DateTime.Now, s);
                }
                
            }
        }
    }





}

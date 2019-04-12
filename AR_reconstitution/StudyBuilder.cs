using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Packaging;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AR_reconstitution
{
    public class StudyBuilder
    {

        public List<String> Journal { get; private set; } = new List<string>();
        public List<ITypeConversion> ConversionTool =  new List<ITypeConversion>();

        public StudyBuilder()
        {
            this.ConversionTool.Add(new TypeConversion<Cartouche>("cartouche","cartouche"));
            this.ConversionTool.Add(new TypeConversion<Consolidation>("consolidation", "consolidation"));
            this.ConversionTool.Add(new TypeConversion<SubStas>("substations", "substations"));
            this.ConversionTool.Add(new TypeConversion<Composteurs>("composteurs", "composteurs"));
            
        }

        public bool BuildFromDirectory(String pathDirectory)
        {
            this.Journal.Clear();
            if (!Directory.Exists(pathDirectory))
            {
                this.Journal.Add("Erreur : Impossible de trouver le repertoire d'etude");
                return false;
            }

            String l_cartouchePath = Path.Combine(pathDirectory, "cartouche.xml");
            if (!File.Exists(l_cartouchePath))
            {
                this.Journal.Add("Erreur : Impossible de trouver le fichier cartouche");
                return false;
            }
            String l_outfile = null;
            using (StreamReader l_readerCart = new StreamReader(l_cartouchePath))
            {
                try
                {
                    XmlSerializer SerializerCart = new XmlSerializer(typeof(Cartouche));
                    Cartouche eltCartouche = (Cartouche) SerializerCart.Deserialize(l_readerCart);

                    l_outfile = Path.Combine(pathDirectory, eltCartouche.Info.Nom);
                    switch (eltCartouche.Info.Etape)
                    {
                        case 1:
                            // Cas d'une étude à rezipper
                            l_outfile += ".etd";
                            break;

                        case 2:
                            // cas d'une campagne à rezipper
                            l_outfile += ".cmp";
                            break;
                    }
                }
                catch (Exception ex)
                {
                    this.Journal.Add("Impossible de deserialiser le fichier cartouche : " + ex.Message);
                    return false;

                }

            }
            if (String.IsNullOrEmpty(l_outfile))
            {
                this.Journal.Add("Le fichier de sortie n'a pas pu être créé");
                return false;
            }
            this.Journal.Add("Construction du nom du fichier : " + l_outfile);
            String fileRelation = Path.Combine(pathDirectory, "_rels", ".rels");
            if (File.Exists(fileRelation))
            {
                this.Journal.Add("Construction du fichier avec le fichier .rels");
                return this.BuildFromDirectoryWithRels(pathDirectory, fileRelation, l_outfile);
          ;
            }
            else
            {
                this.Journal.Add("Construction du fichier sans le fichier .rels");
                return this.BuildFromDirectory(pathDirectory, l_outfile);
           
            }

        }

        public bool BuildFromDirectory(String directory, String outName)
        {
            String[] lFiles = Directory.GetFiles(directory, "*", SearchOption.TopDirectoryOnly);
            bool haswork = false;
            Package myzipFile = ZipPackage.Open(outName, FileMode.Create);
            foreach (string lFile in lFiles)
            {
                ITypeConversion lConversion = this.ConversionTool.FirstOrDefault(c => c.IsConserned(lFile));
                if (lConversion == null)
                {
                    this.Journal.Add("Aucun element de conversion pour le fichier "+ lFile);
                    continue;
                }
                haswork = true;


                try
                {
                    StreamReader l_rltReader = new StreamReader(lFile);
                    String localPath = Path.GetFileName(lFile);
                    Uri zipPartUri = PackUriHelper.CreatePartUri(new Uri(localPath, UriKind.Relative));

                    // <création partition config
                    PackagePart configsPart = myzipFile.CreatePart(zipPartUri, System.Net.Mime.MediaTypeNames.Text.Xml, CompressionOption.Normal);
                    StreamWriter streamWriter = new StreamWriter(configsPart.GetStream(FileMode.Create));

                    streamWriter.Write(l_rltReader.ReadToEnd());
                    streamWriter.Close();
                    l_rltReader.Close();

                    myzipFile.CreateRelationship(configsPart.Uri, TargetMode.Internal, lConversion.TypeElts, lConversion.Relation);
                }
                catch (Exception ex)
                {
                    this.Journal.Add("impossible d'ecrire le fichier: " + lFile + " : " + ex.Message);

                }
            }

            myzipFile.Close();

            return haswork;
        }

        public bool BuildFromDirectoryWithRels(String directory, String pathFileRels, String outName)
        {
            Relationships eltRlts = null;
            try
            {
                StreamReader l_reader = new StreamReader(pathFileRels);
                XmlSerializer SerializerObj = new XmlSerializer(typeof(Relationships));
                eltRlts = (Relationships)SerializerObj.Deserialize(l_reader);
            }
            catch (Exception ex )
            {

                this.Journal.Add("Impossible de deserialiser le fichier rels : " + ex.Message);
                return false;
            }
            if (eltRlts == null || ! eltRlts.Relationship.Any())
            {
                this.Journal.Add("le fichier rels est vide !");
                return false;
            }
            Package myzipFile = ZipPackage.Open(outName, FileMode.Create);

            foreach (RelationshipsRelationship irlt in eltRlts.Relationship)
            {
                // Récup Nom fichier à packager

                String l_pathFile = Path.Combine(Directory.GetCurrentDirectory(), irlt.Target);
                try
                {
                    StreamReader l_rltReader = new StreamReader(directory + l_pathFile);

                    Uri zipPartUri = PackUriHelper.CreatePartUri(new Uri(irlt.Target, UriKind.Relative));

                    // <création partition config
                    PackagePart configsPart = myzipFile.CreatePart(zipPartUri, System.Net.Mime.MediaTypeNames.Text.Xml, CompressionOption.Normal);
                    StreamWriter streamWriter = new StreamWriter(configsPart.GetStream(FileMode.Create));

                    streamWriter.Write(l_rltReader.ReadToEnd());
                    streamWriter.Close();
                    l_rltReader.Close();

                    myzipFile.CreateRelationship(configsPart.Uri, TargetMode.Internal, irlt.Type, irlt.Id);
                }
                catch (Exception ex )
                {
                    this.Journal.Add("impossible d'ecrire le fichier: " + l_pathFile +" : "+ex.Message);
             
                }
                

                this.Journal.Add("Ecriture du fichier: " + l_pathFile);

            }
            myzipFile.Close();


            return true;
        }


         
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;
using System.Windows.Forms;

namespace PopCornToAtelierRadio
{

    public struct JournalLine 
    {
        public String m_line;
        public bool m_showLine;
        public int m_occurence;

        public JournalLine(String _line, bool _isShown)
        {
            m_line = _line;
            m_showLine= _isShown;
            m_occurence = 1;

        }
    }

    public class PopToRadioTranslator
    {
        public bool FileWritten { get; set; }
        public PopToRadioTranslator(String _nameFile, String _path, String _out, List<JournalLine> _journal)
        {
            String l_nameEtu =  Path.GetFileName(_nameFile);
            if (l_nameEtu.Count() > 29)
            {
                l_nameEtu = l_nameEtu.Substring(0, 29);
            }

            m_pathSourceStation = _path+"\\ZONE\\";
            m_journal = _journal;


            this.CreateCorrespondence();

            m_attrib = new Dictionary<int, Dictionary<string, string>>();

            this.FileWritten = false;

            campagne eltPop = this.GetPopFile(_nameFile);
            if (eltPop != null)
            {
                this.FillStudyAccordingtoPopFile(_path, l_nameEtu, eltPop);

                String l_nameFile;
                XmlSerializer serializer = new XmlSerializer(m_study.GetType());
                if (_out == "")
                {
                    l_nameFile = l_nameEtu;
                }
                else
                {
                    l_nameFile = _out + "\\" + l_nameEtu;
                }

                TranslateXmlToZip l_translate = new TranslateXmlToZip(l_nameFile, m_study);

                /*
                StreamWriter file = new StreamWriter(l_nameFile);
                serializer.Serialize(file, m_study);

                file.Close();
                 */ 
                this.FileWritten = true;


            }
            else
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Study));

                Study eltAr = null;

                try
                {
                    StreamReader file = new StreamReader(_nameFile);



                    eltAr = (Study)serializer.Deserialize(file);
                    file.Close();
                    String outFle;
                    l_nameEtu = Path.GetFileNameWithoutExtension(l_nameEtu);

                   
                   // String l_path = Path.GetFullPath(l_nameEtu);


                    if (_out == "")
                    {
                        outFle = l_nameEtu;
                    }
                    else
                    {
                        outFle = _out + "\\" + l_nameEtu;
                    }
                    outFle += ".etd";

                    if (eltAr.Root_Cartouche != null)
                    {
                        NumberStudy.FillNumber(_path, eltAr.Root_Cartouche);
                    }
                        

                    TranslateXmlToZip l_translate = new TranslateXmlToZip(outFle, eltAr);

                    this.FileWritten = true;

                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

        }
        private void FillStudyAccordingtoPopFile(String _pathRadio, String _nameEtu, campagne eltPop)
        {
            this.CreateEmptyStudy(_pathRadio, _nameEtu);

            m_study.Root_Cartouche.Date.DateEtudeValide = this.GetDate(this.UintToDateTime(eltPop.start));
            m_study.Root_Cartouche.Periode.DateDebutPeriodeA = this.GetDate(this.GetMonday(this.UintToDateTime(eltPop.start)));

            m_datePlan = this.GetMonday(this.UintToDateTime(eltPop.start));

            int deltaDays = (int)this.UintToDateTime(eltPop.end).Subtract(this.GetMonday(this.UintToDateTime(eltPop.start))).TotalDays;

            int nbSemaine = (deltaDays / 7) + 1;

            m_study.Root_Cartouche.Periode.NbSemainePeriodeA = (ushort)nbSemaine;

            this.AddEmptyPlan(eltPop.name);

            //format
            m_study.Root_Cartouche.blocFormats.Format = new StudyRoot_CartoucheBlocFormatsFormat[eltPop.format.Count()]; //;
            m_study.Root_Plans.Plan.Data.blocFormats.Format = new StudyRoot_PlansPlanDataBlocFormatsFormat[eltPop.format.Count()];

            m_indexFormats = new Dictionary<int, List<int>>();
            m_currentIndexFormat = 0;
            for (int i = 0; i < eltPop.format.Count(); ++i)
            {
                this.AddFormat(eltPop.format[i].duration, m_currentIndexFormat, false);
                List<int> myInitindexFormatList = new List<int>();
                myInitindexFormatList.Add(m_currentIndexFormat);
                m_indexFormats.Add(eltPop.format[i].duration, myInitindexFormatList);
                ++m_currentIndexFormat;
            }
            //stations
            int nbStation = eltPop.version.periode.station.Count();
            int l_nbStationValide = 0;
            // index pop | index de boucle AR
            Dictionary<int, int> l_indexValidPop = new Dictionary<int, int>();
            for (int i = 0; i < nbStation; ++i)
            {
                if (m_correspIndexStations.ContainsKey(eltPop.version.periode.station[i].code))
                {
                    l_indexValidPop.Add(i, l_nbStationValide);
                    ++l_nbStationValide;
                }
                // this.AddStation(m_study,);
            }


            m_study.Root_Cartouche.blocStations.GroupeStation.Stations = new StudyRoot_CartoucheBlocStationsGroupeStationStations[1];
            m_study.Root_Cartouche.blocStations.GroupeStation.Stations[0] = new StudyRoot_CartoucheBlocStationsGroupeStationStations();
            m_study.Root_Cartouche.blocStations.GroupeStation.Stations[0].CodeReg = 0;
            m_study.Root_Cartouche.blocStations.GroupeStation.Stations[0].Station = new StudyRoot_CartoucheBlocStationsGroupeStationStationsStation[l_nbStationValide];

            m_study.Root_Plans.Plan.Data.blocRegions.Region = new StudyRoot_PlansPlanDataBlocRegionsRegion(); // a changer
            m_study.Root_Plans.Plan.Data.blocRegions.Region.Station = new StudyRoot_PlansPlanDataBlocRegionsRegionStation[l_nbStationValide];


            //tarif liés à la station
            m_study.Root_Cartouche.blocStations.GroupeStation.LienTarifs = new StudyRoot_CartoucheBlocStationsGroupeStationLienTarifs[1];
            m_study.Root_Cartouche.blocStations.GroupeStation.LienTarifs[0] = new StudyRoot_CartoucheBlocStationsGroupeStationLienTarifs();
            m_study.Root_Cartouche.blocStations.GroupeStation.LienTarifs[0].CodeReg = 0;
            m_study.Root_Cartouche.blocStations.GroupeStation.LienTarifs[0].RegStaTarif = new StudyRoot_CartoucheBlocStationsGroupeStationLienTarifsRegStaTarif[l_nbStationValide];
            for (int i = 0; i < nbStation; ++i)
            {
                if (m_correspIndexStations.ContainsKey(eltPop.version.periode.station[i].code))
                {
                    this.AddStation(m_correspIndexStations[eltPop.version.periode.station[i].code], l_indexValidPop[i], eltPop.version.periode.station[i].code);
   
                }
                // this.AddStation(m_study,);
            }
            

            int nbSpots = eltPop.version.spot.Count();
            m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents = new StudyRoot_PlansPlanDataBlocSpotsSP[nbSpots];

            m_journal.Add(new JournalLine("traitement :  " + _nameEtu + " :", false));


            for (int i = 0; i < nbSpots; ++i)
            {
                campagneVersionSpot sp = eltPop.version.spot[i];

                //en attente
                string l_modulation = "";
                List<String> l_modulationList = new List<String>();

                if (sp.modulation != null)
                {
                    for (int j = 0; j < sp.modulation.Count(); ++j)
                    {
                        l_modulation = sp.modulation[j].codeedi;
                        if (l_modulation.Count() == 0)
                        {
                            l_modulation = sp.modulation[j].code;

                        }
                        l_modulationList.Add(l_modulation);
                    }
                }
                this.AddSpot(sp.station, sp.duration, sp.startuv, i, this.UintToDateTime(sp.date), l_modulationList);
            }
        }


        private campagne GetPopFile(String l_nameEtu)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(campagne));

            campagne eltPop = null;

            try
            {
                StreamReader file = new StreamReader(l_nameEtu);

               

                eltPop = (campagne)serializer.Deserialize(file);
                file.Close();
            }
            catch (System.Exception ex)
            {
                m_journal.Add(new JournalLine(l_nameEtu + " non traité",true));
                m_journal.Add(new JournalLine("  ex : "+ex.Message, false));
            }
           

           // eltPop.version.spot.
            

            return eltPop;
        }
        private void AddStation( int _codeSta,int _index, int _indexPop)
        {
            m_study.Root_Cartouche.blocStations.GroupeStation.Stations[0].Station[_index] = new StudyRoot_CartoucheBlocStationsGroupeStationStationsStation();
            m_study.Root_Cartouche.blocStations.GroupeStation.Stations[0].Station[_index].Code = (short)_codeSta;

            m_study.Root_Plans.Plan.Data.blocRegions.Region.Station[_index] = new StudyRoot_PlansPlanDataBlocRegionsRegionStation(); // a changer ajouter la possibilite d avoir plusieurs region
            m_study.Root_Plans.Plan.Data.blocRegions.Region.Station[_index].Sta = _codeSta;


            StudyRoot_CartoucheBlocStationsGroupeStationLienTarifsRegStaTarif l_currentTarf = new StudyRoot_CartoucheBlocStationsGroupeStationLienTarifsRegStaTarif();

            l_currentTarf.TypeEvolutif = 0;
            l_currentTarf.Date = 0;
            l_currentTarf.NumeroTarif = 0;
            l_currentTarf.CodeTarif = "NSTRD";
            l_currentTarf.TxMGaranti = 0;

            m_study.Root_Cartouche.blocStations.GroupeStation.LienTarifs[0].RegStaTarif[_index] = l_currentTarf;

            if (!this.CreateAttribForStation(_codeSta))
            {
                String l_text = "Station pop "+_indexPop.ToString()+" : Attention aucun attribut remonté car aucune correspondance dans l'Atelier Radio a été trouvée";
                JournalLine eltJourn = m_journal.Find(jl => jl.m_line == l_text);
                if (eltJourn.m_occurence == 0)
                {
                    m_journal.Add(new JournalLine(l_text, true));
                }
                else
                {
                    ++eltJourn.m_occurence;
                }
                                   




            }
            


        }
        private bool AddSpot(int _indexStation, int _spotDuration,  ushort horraire, int _indexSpot, DateTime _dateSpot,List<String> _edi)
        {
            bool success = false;


            if (m_correspIndexStations.ContainsKey(_indexStation) && m_indexFormats.ContainsKey(_spotDuration))
            {
                byte l_day = (byte) _dateSpot.Subtract(m_datePlan).TotalDays;
                byte l_stationId = (byte) m_correspIndexStations[_indexStation];
                //byte l_numForm = (byte)m_indexFormats[_spotDuration];

                List<int> indexFormatByFormat = m_indexFormats[_spotDuration];
                int indexFound = -1;
          
                    foreach (int l_index in indexFormatByFormat)
                    {

                        IEnumerable<StudyRoot_PlansPlanDataBlocSpotsSP> eltSpot = m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents.Where(sp => (sp!=null)&&(sp.Sta == l_stationId) && (sp.Jr == l_day) && (sp.Hor == horraire) && (sp.NoFor == l_index));
                        if (eltSpot.Count() == 0)
                        {
                            indexFound = l_index;
                            break;
                        }

                    }
                    if (indexFound == -1)
                    {
                        AddFormat(_spotDuration, m_currentIndexFormat,true);
                        indexFormatByFormat.Add(m_currentIndexFormat);
                        indexFound = m_currentIndexFormat;
                        ++m_currentIndexFormat;
                    }


                
          

                
                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot] = new StudyRoot_PlansPlanDataBlocSpotsSP();
                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].Hor = horraire;

                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].Num =  _indexSpot;
                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].NoFor = (byte)indexFound;
                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].Sta = m_correspIndexStations[_indexStation];
                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].ValAtt = 1;
                m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].Jr =(byte) _dateSpot.Subtract(m_datePlan).TotalDays;

                int l_indexStation = m_correspIndexStations[_indexStation];

                if (_edi.Count > 0)
                {
                    String l_Attrib = "";
                    byte l_valatt = 0;

                    foreach (String stEdi in _edi)
                    {


                        if (m_attrib.ContainsKey(l_indexStation))
                        {



                            if (m_attrib[l_indexStation].ContainsKey(stEdi))
                            {
                                l_Attrib += m_attrib[l_indexStation][stEdi]+",";

                                if (m_correspAttrib.ContainsKey(stEdi))
                                {
                                    l_valatt += (byte)m_correspAttrib[stEdi];
                                }

                            }
                            else
                            {
                                String l_text = " attibut non traité " + stEdi + " sur station " + l_indexStation.ToString()  ;


                                try
                                {
                                    JournalLine eltJourn = m_journal.Find(jl => jl.m_line == l_text);
                                    if (eltJourn.m_occurence == 0)
                                    {
                                        m_journal.Add(new JournalLine(l_text, false));
                                    }
                                    else
                                    {
                                        ++eltJourn.m_occurence;
                                    }
                                   
                                }
                                catch
                                {
                                    m_journal.Add(new JournalLine(l_text, false));
                                }
                               
                             
                                
                            }

                        }

                    }
                    m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].Attr = l_Attrib;
                    m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].ValAtt += l_valatt;
                }

                success = true;
            }

          
          //  m_study.Root_Plans.Plan.Data.blocSpots.SpotsPresents[_indexSpot].

            return success;
        }


        private void AddFormat(int _length, int _index, bool _resize)
        {

            if (_resize)
            {
                int nbFormat= m_study.Root_Cartouche.blocFormats.Format.Count()+1;

                StudyRoot_CartoucheBlocFormatsFormat[] l_tmpCart = m_study.Root_Cartouche.blocFormats.Format;
                System.Array.Resize(ref l_tmpCart, nbFormat);
                m_study.Root_Cartouche.blocFormats.Format = l_tmpCart;

                StudyRoot_PlansPlanDataBlocFormatsFormat[] l_tmpPla = m_study.Root_Plans.Plan.Data.blocFormats.Format;
                System.Array.Resize(ref l_tmpPla, nbFormat);
                m_study.Root_Plans.Plan.Data.blocFormats.Format = l_tmpPla;

            }

            m_study.Root_Cartouche.blocFormats.Format[_index] = new StudyRoot_CartoucheBlocFormatsFormat();
            m_study.Root_Cartouche.blocFormats.Format[_index].NbFois = 1;
            m_study.Root_Cartouche.blocFormats.Format[_index].Duree = (byte)_length;
            m_study.Root_Cartouche.blocFormats.Format[_index].Index = (byte)_index;

           
           

            m_study.Root_Plans.Plan.Data.blocFormats.Format[_index] = new StudyRoot_PlansPlanDataBlocFormatsFormat();
            m_study.Root_Plans.Plan.Data.blocFormats.Format[_index].Nom = (byte)_length;


        }
         private void AddEmptyPlan(String _namePlan)
        {
            m_study.Root_Plans = new StudyRoot_Plans();
            m_study.Root_Plans.Plan = new StudyRoot_PlansPlan();

            m_study.Root_Plans.Plan.NrEtude =  m_study.Root_Cartouche.Information.NumeroEtu;

            // Modif Alain  (on chargera le plan directement dans le creuset) 
            // m_study.Root_Plans.Plan.Occupe = 1;
            m_study.Root_Plans.Plan.Occupe = 0;
            m_study.Root_Plans.Plan.NrPlan = 1;

            m_study.Root_Plans.Plan.Nom = _namePlan;
            m_study.Root_Plans.Plan.NumCible = -1;
            m_study.Root_Plans.Plan.DateDebut = (ushort)m_study.Root_Cartouche.Periode.DateDebutPeriodeA;
            m_study.Root_Plans.Plan.DateDebutPA = (ushort)m_study.Root_Plans.Plan.DateDebut;

            m_study.Root_Plans.Plan.NbSemainePA = (byte)m_study.Root_Cartouche.Periode.NbSemainePeriodeA; 

             m_study.Root_Plans.Plan.DateFin = (ushort)((m_study.Root_Plans.Plan.DateDebut+ 7*(ushort)(m_study.Root_Plans.Plan.NbSemainePA)) - 1);
             m_study.Root_Plans.Plan.DateFinPA = (ushort)((m_study.Root_Plans.Plan.DateDebutPA+ 7* (ushort)(m_study.Root_Plans.Plan.NbSemainePA)) - 1);

             m_study.Root_Plans.Plan.Data = new StudyRoot_PlansPlanData();
             m_study.Root_Plans.Plan.Data.blocRegions = new StudyRoot_PlansPlanDataBlocRegions();
             m_study.Root_Plans.Plan.Data.blocStrates = new StudyRoot_PlansPlanDataBlocStrates();
             m_study.Root_Plans.Plan.Data.blocStrates.CurStrate = 0;
             m_study.Root_Plans.Plan.Data.blocStrates.Strate = new StudyRoot_PlansPlanDataBlocStratesStrate();

             m_study.Root_Plans.Plan.Data.blocStrates.Strate.Code = 0;
             m_study.Root_Plans.Plan.Data.blocStrates.Strate.Mode = 0;
             m_study.Root_Plans.Plan.Data.blocStrates.Strate.No = 0;
             m_study.Root_Plans.Plan.Data.blocStrates.Strate.Nom = "Cl";

             m_study.Root_Plans.Plan.Data.blocFormats = new StudyRoot_PlansPlanDataBlocFormats();
             m_study.Root_Plans.Plan.Data.blocFormats.CurFormat = 0;

             m_study.Root_Plans.Plan.Data.blocSpots = new StudyRoot_PlansPlanDataBlocSpots();
          
             //fill later


        }


        private void CreateEmptyStudy(String _pathRadio, String l_nameEtu)
        {
            m_study = new Study();

            m_study.FileVersion = 1;

            StudyRoot_Cartouche l_cartouche = new StudyRoot_Cartouche();

            l_cartouche.Information = new StudyRoot_CartoucheInformation();
            l_cartouche.Information.Nom = Path.GetFileNameWithoutExtension(l_nameEtu);

            // Modif ALAIN (lecture Compteur en cours et récup dernier n° étude dispo)

            NumberStudy.FillNumber(_pathRadio, l_cartouche);

            // Par défaut c'est une étude
            l_cartouche.Information.Etape = 1; //1 etude

             l_cartouche.Date = new StudyRoot_CartoucheDate();
             l_cartouche.Date.DateDebut = -1;
             l_cartouche.Date.DateFin = -1;
             l_cartouche.Date.DateFin010180 = 0;
             l_cartouche.Date.DateDebut010180 = 0;
             l_cartouche.Date.DateHeureExtraitConsolide = 0;


             l_cartouche.Date.DateEtudeValide = 0;
             // DateTime l_monday = DateTime.Today.DayOfWeek.

             //to do

             l_cartouche.Periode = new StudyRoot_CartouchePeriode();
             l_cartouche.Periode.NbSemainePeriodeA = 5;
             l_cartouche.Periode.DateDebutPeriodeA = 0;
             //to do

            l_cartouche.Echange = new StudyRoot_CartoucheEchange();
            l_cartouche.Echange.TauxEchange = "6,55957"; //a revoir xsd pour ma machine
         //   l_cartouche.Echange.TauxEchange = "6.55957"; //a revoir xsd pour le client
            l_cartouche.Echange.Devise = 1;

            l_cartouche.Divers = new StudyRoot_CartoucheDivers();
            l_cartouche.Divers.NrMaillage = 0;
            l_cartouche.Divers.AgrJours = "031032064000000000000000";
            l_cartouche.Divers.PlageHoraire = "1000000010000000100000000000100000001000000000000000100000001000000000000000100000000000000000001"; //xsd

            l_cartouche.blocCoches = new StudyRoot_CartoucheBlocCoches();
            l_cartouche.blocCoches.EtatSel = "6626666666000000000000000";

            l_cartouche.blocVagues = new StudyRoot_CartoucheBlocVagues();
            l_cartouche.blocVagues.VagueUtil =new object();
            l_cartouche.blocVagues.VagueRefSource = new StudyRoot_CartoucheBlocVaguesVagueRefSource();
            l_cartouche.blocVagues.VagueRefSource.Vague = new StudyRoot_CartoucheBlocVaguesVagueRefSourceVague();
            l_cartouche.blocVagues.VagueRefSource.Vague.Code = 0;

            l_cartouche.blocStations =new StudyRoot_CartoucheBlocStations();
            l_cartouche.blocStations.GroupeStation = new StudyRoot_CartoucheBlocStationsGroupeStation();
            l_cartouche.blocStations.GroupeStation.Nom = "A";
            l_cartouche.blocStations.GroupeStation.NumeroMaillage = 1;

            l_cartouche.blocListeStaPasHeure = new object();
            l_cartouche.blocListeStaPasEcran = new object();
            l_cartouche.blocListeCorGrp = new object();

            l_cartouche.blocFormats = new StudyRoot_CartoucheBlocFormats();
            l_cartouche.blocFormats.BaseFormat = 0;

            l_cartouche.blocFormats = new StudyRoot_CartoucheBlocFormats();
            l_cartouche.blocFormats.BaseFormat = 0;

            l_cartouche.blocFormatsSpecifs = new object();

            l_cartouche.blocStrates = new StudyRoot_CartoucheBlocStrates();
            l_cartouche.blocStrates.Strate = new StudyRoot_CartoucheBlocStratesStrate();
            l_cartouche.blocStrates.Strate.Index = 0;
            l_cartouche.blocStrates.Strate.Numero = 0;
            l_cartouche.blocStrates.Strate.NbFois = 1;

            l_cartouche.blocEtudeOption = new StudyRoot_CartoucheBlocEtudeOption();
            l_cartouche.blocEtudeOption.OptionClassement = new StudyRoot_CartoucheBlocEtudeOptionOptionClassement();
            l_cartouche.blocEtudeOption.OptionClassement.fClasMixte = 0;
            l_cartouche.blocEtudeOption.OptionPoids = new StudyRoot_CartoucheBlocEtudeOptionOptionPoids();
            l_cartouche.blocEtudeOption.OptionPoids.PoidsPui = 100;
            l_cartouche.blocEtudeOption.OptionPoids.PoidsEco = 100;
            l_cartouche.blocEtudeOption.OptionPoids.PoidsAff = 0;
            l_cartouche.blocEtudeOption.OptionAffinite = new StudyRoot_CartoucheBlocEtudeOptionOptionAffinite();
            l_cartouche.blocEtudeOption.OptionAffinite.NrCibleBaseAff = -1;
            l_cartouche.blocEtudeOption.OptionAffinite.fAffIndice = 1;
            l_cartouche.blocEtudeOption.OptionTarif = new StudyRoot_CartoucheBlocEtudeOptionOptionTarif();
            l_cartouche.blocEtudeOption.OptionTarif.PTarifFacial = 1;
            l_cartouche.blocEtudeOption.OptionCalcul = new StudyRoot_CartoucheBlocEtudeOptionOptionCalcul();
            l_cartouche.blocEtudeOption.OptionCalcul.fMoyenneGRP = 2;
            l_cartouche.blocEtudeOption.OptionCalcul.fPenetration = 0;
            l_cartouche.blocEtudeOption.OptionCalcul.ValueMoyenneGRP = 0;//"0000000000000000000000000000000000000000000000000"
            l_cartouche.blocEtudeOption.OptionEdition = new StudyRoot_CartoucheBlocEtudeOptionOptionEdition();
            l_cartouche.blocEtudeOption.OptionEdition.fLigneGrisee = 1;
            l_cartouche.blocEtudeOption.OptionEdition.fPleinEcran = 0;

           // l_cartouche.blocCampagne;

            l_cartouche.blocTarification = new StudyRoot_CartoucheBlocTarification();
            l_cartouche.blocTarification.BaseFormat = 0;
            l_cartouche.blocTarification.blocPrixBiem = new object();

            l_cartouche.blocTitrePerso = new StudyRoot_CartoucheBlocTitrePerso();
            l_cartouche.blocTitrePerso.LgTitPersonalise = 0;
            l_cartouche.blocTitrePerso.TitPersonalise = "";

            l_cartouche.blocSpecifYacast = new StudyRoot_CartoucheBlocSpecifYacast();
            l_cartouche.blocSpecifYacast.IdNomenTar = 0;
            l_cartouche.blocSpecifYacast.NivNomenTar = 0;

            m_study.Root_Cartouche = l_cartouche;

        }

        private void CreateCorrespondence()
        {

            m_correspIndexStations = new Dictionary<int, int>();

            m_correspIndexStations.Add(1, 101);
            m_correspIndexStations.Add(2, 108);
            m_correspIndexStations.Add(4, 111);
            m_correspIndexStations.Add(5, 106);
            m_correspIndexStations.Add(6, 107);
            m_correspIndexStations.Add(7, 109);
            m_correspIndexStations.Add(8, 103);
            m_correspIndexStations.Add(9, 102);
            m_correspIndexStations.Add(10, 110);
            m_correspIndexStations.Add(13, 113);
            m_correspIndexStations.Add(14, 112);
            m_correspIndexStations.Add(28, 122);
            m_correspIndexStations.Add(30, 117);
            m_correspIndexStations.Add(32, 121);
            m_correspIndexStations.Add(35, 123);
            m_correspIndexStations.Add(90, 149);
            m_correspIndexStations.Add(78, 355);
             m_correspIndexStations.Add(144, 356);
         

            m_correspAttrib = new Dictionary<String, int>();

            m_correspAttrib.Add("EP", 2);
            m_correspAttrib.Add("EPP", 2);
            m_correspAttrib.Add("P1000", 2);
            m_correspAttrib.Add("P2000", 2);
            m_correspAttrib.Add("P3000", 2);
            m_correspAttrib.Add("P0008", 2);
            m_correspAttrib.Add("P0009", 2);
            m_correspAttrib.Add("P1209", 2);
            m_correspAttrib.Add("ECH", 4);
            m_correspAttrib.Add("GPA", 8);
            m_correspAttrib.Add("GPC", 32);
            m_correspAttrib.Add("GCV", 64);
            m_correspAttrib.Add("ECA", 256);
            m_correspAttrib.Add("BRC", 512);
            m_correspAttrib.Add("BRT", 512);
            m_correspAttrib.Add("BRE", 512);
        }

        private int GetDate(DateTime _date)
        {
            DateTime l_ref = new DateTime(1980, 1, 1);
            return (int)(_date.Subtract(l_ref).TotalDays + 1);

        }
        private DateTime GetMonday(DateTime _date)
        {
            int offset = _date.DayOfWeek - DayOfWeek.Monday;
            DateTime l_monday = _date.AddDays(-offset);

            return l_monday;

        }

        private DateTime UintToDateTime(uint _date)
        {
            int annee = (int)_date / 10000;

            int restant = (int)_date % 10000;

            int month = restant / 100;

            int day = restant % 100;


            //date
            DateTime creationdate = new DateTime(annee, month, day);

            return creationdate;
        }

        private bool CreateAttribForStation(int _code)
        {
            String pathfile = m_pathSourceStation+"\\ZFR00\\C00"+_code.ToString()+"\\Frmodult_2.tbl";
            bool success = false;
            if (File.Exists(pathfile))
            {
                TextReader l_reader = new StreamReader(pathfile, Encoding.GetEncoding("iso-8859-1"));
                string currentLine;

                Dictionary<String, String> eltStation = new Dictionary<String, String>(); 

                while ((currentLine = l_reader.ReadLine()) != null)
                {
                    if (currentLine.Length > 0)
                    {
                        if (currentLine[0] != '#')
                        {
                            char separator = ';';

                            string[] eltString = currentLine.Split(separator);

                            string l_edi;
                            eltString[2] = eltString[2].Trim();
                            switch (eltString[2])
                            {
                                case "EREDP":
                                case "EP":
                                    l_edi = eltString[4].Trim();
                            	    break;

                                default:
                                    l_edi = eltString[2].Trim();
                                    break;

                            }
                            if (l_edi.Trim().Count() > 0)
                            {
                                separator = ' ';
                                String[] eltFirstLine = eltString[0].Split(separator);
                                IEnumerable<String> eltNonNull = eltFirstLine.Where(s => s.Count() > 0);

                                string l_first;

                                if (eltNonNull.ElementAt(2).Equals("3"))
                                {
                                    l_first = "G";
                                }
                                else
                                {
                                    l_first = "M";
                                }

                                string eltLineAttrib = l_first + eltFirstLine[0] + "/0/2/" + eltNonNull.ElementAt(3) + "/" + eltString[1];

                                if (!eltStation.ContainsKey(l_edi))
                                {
                                    eltStation.Add(l_edi, eltLineAttrib);
                                }

                            }

                        }
                    }
                    string att = currentLine;
                }
                m_attrib.Add(_code, eltStation);
                success = true;
            }
            else
            {
                m_journal.Add(new JournalLine("  st :"+_code.ToString()+ " aucun fichier Atelier Radio  " + Path.GetDirectoryName(pathfile),false));
            }

            return success;
        }

        String m_pathSourceStation;
        

        Dictionary<int, int> m_correspIndexStations;
        Dictionary<String, int> m_correspAttrib;
        Dictionary<int, List<int>> m_indexFormats;
        int m_currentIndexFormat;

        Dictionary<int, Dictionary<string, string>> m_attrib;
        List<JournalLine> m_journal;

        Study m_study;

        DateTime m_datePlan;
    }
}

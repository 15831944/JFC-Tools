using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GestionXmlAtelierRadio;
using System.IO.Packaging;
using System.IO;
using System.Xml.Serialization;
using System.Xml;

namespace PopCornToAtelierRadio
{
    class TranslateXmlToZip
    {
        public TranslateXmlToZip (String _filePath, Study _study)
        {
            // il n'y a  pas de correspondance directe entre les objets provenant de la version Accenture et la version nouveau format

            
            Package l_package = ZipPackage.Open(_filePath, FileMode.Create);
 

            //********** cartouche **************//
            Cartouche l_cartouche = this.ConvertCartouche(_study.Root_Cartouche);

            Uri zipPartUri = PackUriHelper.CreatePartUri(new Uri("cartouche.xml", UriKind.Relative));
            PackagePart cartouchePart = l_package.CreatePart(zipPartUri, System.Net.Mime.MediaTypeNames.Text.Xml, CompressionOption.Normal);

            XmlSerializer serializer = new XmlSerializer(typeof(Cartouche));
            serializer.Serialize(cartouchePart.GetStream(), l_cartouche);
            l_package.CreateRelationship(cartouchePart.Uri, TargetMode.Internal, "cartouche", "cartouche");

            if (_study.Root_Plans.Plan != null)
            {
                String l_filePlan = GestionXmlAtelierRadio.PlanFileNameProvider.Provide(_study.Root_Plans.Plan.Occupe, _study.Root_Plans.Plan.NrPlan);
                Uri zipPartUriPlan = PackUriHelper.CreatePartUri(new Uri(l_filePlan, UriKind.Relative));
                PackagePart planpart = l_package.CreatePart(zipPartUriPlan, System.Net.Mime.MediaTypeNames.Text.Xml, CompressionOption.Normal);

                PlansPlan l_plan = this.ConvertPlan(_study.Root_Plans.Plan);

                XmlSerializer serializerPlan = new XmlSerializer(typeof(PlansPlan));
                serializerPlan.Serialize(planpart.GetStream(), l_plan);
                l_package.CreateRelationship(cartouchePart.Uri, TargetMode.Internal, "plan", l_filePlan);




            }
           // _study.Root_Plans.


            l_package.Close();

         
        }

        private PlansPlan ConvertPlan(StudyRoot_PlansPlan _plan)
        {
            PlansPlan l_plan = new PlansPlan();
            l_plan.NrEtude = _plan.NrEtude;
            l_plan.NrPlan = _plan.NrPlan;
            l_plan.Version = _plan.Version;
            l_plan.Nom = _plan.Nom;
            l_plan.RefPlan = _plan.RefPlan;
            l_plan.Occupe = _plan.Occupe;
            l_plan.NoSas = _plan.NoSas;
            l_plan.DateDebut = _plan.DateDebut;
            l_plan.DateFin = _plan.DateFin;
            l_plan.NoCib = _plan.NumCible;
            l_plan.NomCib = _plan.NomCible;
            l_plan.NumVague = _plan.NumVague;
            l_plan.NomVague = _plan.NomVague;
            l_plan.DateHeureMemo = _plan.DateHeureMemo;
            l_plan.NbSemainePA = _plan.NbSemainePA;
            l_plan.DateFinPA = _plan.DateFinPA;
            l_plan.DateDebutPA = _plan.DateDebutPA;
            l_plan.Open = 0;

            l_plan.Data = new PlansPlanData();
            l_plan.Data.blocRegs = new PlansPlanDataBlocRegs();
            l_plan.Data.blocRegs.CurRegion = _plan.Data.blocRegions.CurRegion;
            l_plan.Data.blocRegs.CurStation = _plan.Data.blocRegions.CurStation;
            l_plan.Data.blocRegs.CurNiveau = _plan.Data.blocRegions.CurNiveau;

            l_plan.Data.blocRegs.Reg = new PlansPlanDataBlocRegsReg[1];//une seule region traitée pour le moment
            l_plan.Data.blocRegs.Reg[0] = new PlansPlanDataBlocRegsReg();
            l_plan.Data.blocRegs.Reg[0].No = 0;

            Int32 l_nbsStation =_plan.Data.blocRegions.Region.Station.Count();
            l_plan.Data.blocRegs.Reg[0].Sta = new PlansPlanDataBlocRegsRegSta[l_nbsStation];

            for (int i = 0; i < l_nbsStation; ++i )
            {
                l_plan.Data.blocRegs.Reg[0].Sta[i] = new PlansPlanDataBlocRegsRegSta();
                l_plan.Data.blocRegs.Reg[0].Sta[i].No = _plan.Data.blocRegions.Region.Station[i].Sta;
                l_plan.Data.blocRegs.Reg[0].Sta[i].Lock = _plan.Data.blocRegions.Region.Station[i].Lock;
            }

            l_plan.Data.blocTaux = new PlansPlanDataBlocTaux();

            l_plan.Data.blocStrs = new PlansPlanDataBlocStrs();
            l_plan.Data.blocStrs.CurStr = 0;
            l_plan.Data.blocStrs.Str = new PlansPlanDataBlocStrsStr[1];
            l_plan.Data.blocStrs.Str[0] = new PlansPlanDataBlocStrsStr();
            l_plan.Data.blocStrs.Str[0].No = 0;
            l_plan.Data.blocStrs.Str[0].Code = 0;
            l_plan.Data.blocStrs.Str[0].Nom = "Cl";

            l_plan.Data.blocFors = new PlansPlanDataBlocFors();
            l_plan.Data.blocFors.CurFor = 0;

            Int32 l_nbFormat = _plan.Data.blocFormats.Format.Count();
            l_plan.Data.blocFors.For = new PlansPlanDataBlocForsFor[l_nbFormat];

            for (int i = 0; i < l_nbFormat; ++i)
            {
                l_plan.Data.blocFors.For[i] = new PlansPlanDataBlocForsFor();
                l_plan.Data.blocFors.For[i].Mode = _plan.Data.blocFormats.Format[i].Mode;
                l_plan.Data.blocFors.For[i].Nom = _plan.Data.blocFormats.Format[i].Nom.ToString();
                l_plan.Data.blocFors.For[i].No = _plan.Data.blocFormats.Format[i].No;
            }
            l_plan.Data.blocSpots = new PlansPlanDataBlocSpots();

            Int32 l_nbSpot = _plan.Data.blocSpots.SpotsPresents.Count();

            l_plan.Data.blocSpots.Spots = new PlansPlanDataBlocSpotsSP[l_nbSpot];

             for (int i = 0; i < l_nbSpot; ++i)
             {
                 l_plan.Data.blocSpots.Spots[i] = new PlansPlanDataBlocSpotsSP();
                 l_plan.Data.blocSpots.Spots[i].Reg = _plan.Data.blocSpots.SpotsPresents[i].Reg;
                 l_plan.Data.blocSpots.Spots[i].Sta = _plan.Data.blocSpots.SpotsPresents[i].Sta;
                 l_plan.Data.blocSpots.Spots[i].No = (ushort)_plan.Data.blocSpots.SpotsPresents[i].Num;
                 l_plan.Data.blocSpots.Spots[i].Hor = _plan.Data.blocSpots.SpotsPresents[i].Hor;
                 l_plan.Data.blocSpots.Spots[i].Jr = _plan.Data.blocSpots.SpotsPresents[i].Jr;
                 l_plan.Data.blocSpots.Spots[i].NoStr = _plan.Data.blocSpots.SpotsPresents[i].NoStr;
                 l_plan.Data.blocSpots.Spots[i].NoFor = _plan.Data.blocSpots.SpotsPresents[i].NoFor;
                 l_plan.Data.blocSpots.Spots[i].ValAtt = _plan.Data.blocSpots.SpotsPresents[i].ValAtt;
                 l_plan.Data.blocSpots.Spots[i].Attr = _plan.Data.blocSpots.SpotsPresents[i].Attr;
             }


            
            






                return l_plan;

        }

        private Cartouche ConvertCartouche(StudyRoot_Cartouche _cart)
        {
            Cartouche l_cartouche = new Cartouche();
            //info
            l_cartouche.Info = new CartoucheInfo();
            l_cartouche.Info.Nom = _cart.Information.Nom;
            l_cartouche.Info.NoEtu = _cart.Information.NumeroEtu;
            l_cartouche.Info.Etape = 1;

            l_cartouche.Date = new CartoucheDate();
            l_cartouche.Date.DateDebut = _cart.Date.DateDebut;
            l_cartouche.Date.DateFin = _cart.Date.DateFin;
            l_cartouche.Date.DateEtudeValide = _cart.Date.DateEtudeValide;
            l_cartouche.Date.DateHeureExtraitConsolide = _cart.Date.DateHeureExtraitConsolide;
            l_cartouche.Date.DateDebut010180 = _cart.Date.DateDebut010180;
            l_cartouche.Date.DateFin010180 = _cart.Date.DateFin010180;

            l_cartouche.Per = new CartouchePer();
            l_cartouche.Per.DateDebutPeriodeA = _cart.Periode.DateDebutPeriodeA;
            l_cartouche.Per.NbSemainePeriodeA = _cart.Periode.NbSemainePeriodeA;

            l_cartouche.Ech = new CartoucheEch();
            l_cartouche.Ech.Devise = _cart.Echange.Devise;
            l_cartouche.Ech.TauxEchange = (System.Decimal)6.55956983566284;

            l_cartouche.Div = new CartoucheDiv();
            l_cartouche.Div.NrMaillage = _cart.Divers.NrMaillage;
            l_cartouche.Div.AgrJours = _cart.Divers.AgrJours;
            l_cartouche.Div.PlageHoraire = _cart.Divers.PlageHoraire;

            l_cartouche.blocCoches = new CartoucheBlocCoches();
            l_cartouche.blocCoches.EtatSel = _cart.blocCoches.EtatSel;

            l_cartouche.blocVags = new CartoucheBlocVags();
            l_cartouche.blocVags.VagUtil = new CartoucheBlocVagsVag[1];

            l_cartouche.blocVags.VagRefSrc = new CartoucheBlocVagsVag1[1];
            l_cartouche.blocVags.VagRefSrc[0] = new CartoucheBlocVagsVag1();
            l_cartouche.blocVags.VagRefSrc[0].Code = 0;
            l_cartouche.blocVags.VagRefSrc[0].InxTab = 0;

            l_cartouche.blocStas = new CartoucheGrpSta[1];
            l_cartouche.blocStas[0] = new CartoucheGrpSta();

            l_cartouche.blocStas[0].Nom = _cart.blocStations.GroupeStation.Nom;
            l_cartouche.blocStas[0].NoMaillage = _cart.blocStations.GroupeStation.NumeroMaillage;
          
            //ici nous ne prendrons en compte qu'une seule region la 0 (dans le cadre de la concersion pop to ar)
            l_cartouche.blocStas[0].Stations = new CartoucheGrpStaStations[1];
            l_cartouche.blocStas[0].Stations[0] = new CartoucheGrpStaStations();
            l_cartouche.blocStas[0].Stations[0].CodeReg = 0; //nationale uniquement pour le moment

            l_cartouche.blocStas[0].LienTars = new CartoucheGrpStaLienTars[1];
            l_cartouche.blocStas[0].LienTars[0] = new CartoucheGrpStaLienTars();
            l_cartouche.blocStas[0].LienTars[0].CodeReg = 0; //nationale uniquement pour le moment
            
            Int32 nbStation = _cart.blocStations.GroupeStation.Stations[0].Station.Count();

            l_cartouche.blocStas[0].Stations[0].Sta = new CartoucheGrpStaStationsSta[nbStation];
            l_cartouche.blocStas[0].LienTars[0].RegStaTar = new CartoucheGrpStaLienTarsRegStaTar[nbStation];

            for (int i = 0; i < nbStation;++i )
            {
                l_cartouche.blocStas[0].Stations[0].Sta[i] = new CartoucheGrpStaStationsSta();
                l_cartouche.blocStas[0].Stations[0].Sta[i].Code=(ushort)_cart.blocStations.GroupeStation.Stations[0].Station[i].Code;

                l_cartouche.blocStas[0].LienTars[0].RegStaTar[i] = new CartoucheGrpStaLienTarsRegStaTar();
                l_cartouche.blocStas[0].LienTars[0].RegStaTar[i].TypeEvolutif = _cart.blocStations.GroupeStation.LienTarifs[0].RegStaTarif[i].TypeEvolutif;
                l_cartouche.blocStas[0].LienTars[0].RegStaTar[i].Date = _cart.blocStations.GroupeStation.LienTarifs[0].RegStaTarif[i].Date;
                l_cartouche.blocStas[0].LienTars[0].RegStaTar[i].NoTar = _cart.blocStations.GroupeStation.LienTarifs[0].RegStaTarif[i].NumeroTarif;
                l_cartouche.blocStas[0].LienTars[0].RegStaTar[i].CodeTar = _cart.blocStations.GroupeStation.LienTarifs[0].RegStaTarif[i].CodeTarif;
                l_cartouche.blocStas[0].LienTars[0].RegStaTar[i].TxMGaranti = _cart.blocStations.GroupeStation.LienTarifs[0].RegStaTarif[i].TxMGaranti;
            }

            l_cartouche.blocFors = new CartoucheBlocFors();
            l_cartouche.blocFors.BaseFormat = _cart.blocFormats.BaseFormat;

            Int32 nbFormat = _cart.blocFormats.Format.Count();
            l_cartouche.blocFors.Format = new CartoucheBlocForsFormat[nbFormat];

            for (int i = 0; i < nbFormat; ++i )
            {
                l_cartouche.blocFors.Format[i] = new CartoucheBlocForsFormat();
                l_cartouche.blocFors.Format[i].Index = _cart.blocFormats.Format[i].Index;
                l_cartouche.blocFors.Format[i].Duree = _cart.blocFormats.Format[i].Duree.ToString();
                l_cartouche.blocFors.Format[i].NbFois = _cart.blocFormats.Format[i].NbFois;
            }

            //une seule strate pour la conversion
            l_cartouche.blocStrs = new CartoucheStr[1];
            l_cartouche.blocStrs[0] = new CartoucheStr();

            l_cartouche.blocStrs[0].Index = _cart.blocStrates.Strate.Index;
            l_cartouche.blocStrs[0].NbFois = _cart.blocStrates.Strate.NbFois;
            l_cartouche.blocStrs[0].No = _cart.blocStrates.Strate.Numero;

            l_cartouche.blocEtuOpt = new CartoucheBlocEtuOpt();
            l_cartouche.blocEtuOpt.OptClass = new CartoucheBlocEtuOptOptClass();
            l_cartouche.blocEtuOpt.OptClass.fClasMixte = _cart.blocEtudeOption.OptionClassement.fClasMixte;
            l_cartouche.blocEtuOpt.OptPds = new CartoucheBlocEtuOptOptPds();
            l_cartouche.blocEtuOpt.OptPds.PoidsPui = _cart.blocEtudeOption.OptionPoids.PoidsPui;
            l_cartouche.blocEtuOpt.OptPds.PoidsEco = _cart.blocEtudeOption.OptionPoids.PoidsEco;
            l_cartouche.blocEtuOpt.OptPds.PoidsAff = _cart.blocEtudeOption.OptionPoids.PoidsAff;
            l_cartouche.blocEtuOpt.OptAff = new CartoucheBlocEtuOptOptAff();
            l_cartouche.blocEtuOpt.OptAff.NrCibleBaseAff = _cart.blocEtudeOption.OptionAffinite.NrCibleBaseAff;
            l_cartouche.blocEtuOpt.OptAff.fAffIndice = _cart.blocEtudeOption.OptionAffinite.fAffIndice;
            l_cartouche.blocEtuOpt.OptTar = new CartoucheBlocEtuOptOptTar();
            l_cartouche.blocEtuOpt.OptTar.PTarFacial = _cart.blocEtudeOption.OptionTarif.PTarifFacial;
            l_cartouche.blocEtuOpt.OptCalc = new CartoucheBlocEtuOptOptCalc();
            l_cartouche.blocEtuOpt.OptCalc.fMoyenneGRP = _cart.blocEtudeOption.OptionCalcul.fMoyenneGRP;
            l_cartouche.blocEtuOpt.OptCalc.fPenetration = _cart.blocEtudeOption.OptionCalcul.fPenetration;
            l_cartouche.blocEtuOpt.OptEdit = new CartoucheBlocEtuOptOptEdit();
            l_cartouche.blocEtuOpt.OptEdit.fLigneGrisee = _cart.blocEtudeOption.OptionEdition.fLigneGrisee;
            l_cartouche.blocEtuOpt.OptEdit.fPleinEcran = _cart.blocEtudeOption.OptionEdition.fPleinEcran;
            l_cartouche.blocEtuOpt.OptEdit.fCoeffMemo = _cart.blocEtudeOption.OptionEdition.fCoeffMemo;

            l_cartouche.blocTar = new CartoucheBlocTar();
            l_cartouche.blocTar.BaseFormat = _cart.blocTarification.BaseFormat;
            l_cartouche.blocTar.AssietteHon = 0; //non traité  dans les versions que l'on converti
            l_cartouche.blocTar.TauxHon = 0;

            l_cartouche.blocTitPerso = new CartoucheBlocTitPerso();
            l_cartouche.blocSpeYac = new CartoucheBlocSpeYac();
            




          //  l_cartouche.
            


            //l_cartouche.blocVags.VagUtil = new CartoucheBlocVagsVag[_cart.blocVagues.VagueRefSource]();




            return l_cartouche;
        }
    }
}

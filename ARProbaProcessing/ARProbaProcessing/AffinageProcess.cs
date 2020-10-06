using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ARProbaProcessing
{
    public class AffinageProcess
    {

        public List<Fushab09Indiv> Fushab09JM(
            int NBSTA,
            int SIGN_LINE_LEN_BEFORE_HAB,
            int NB_STA_ALL_HAB,
            int[] TABRH)
        {
            // C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // C fusion des Habitudes total RADIO oubliees dans SIGN2007.F04
            // C        et rangement des habitudes dans le meme ordre que les audiences
            // C        avec traitement des 'sans habitudes'.
            // C
            // C
            // INCLUDE 'FSUBLIB.FI'
            // C Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
            // PARAMETER (NBSTA=#NB_STA_HAB_NOTO#)

            // C Attention AVANT(#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au début des données de HAB
            // C Attention APRES(#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler à la fin de la ligne des données

            // INTEGER * 1 AVANT(#SIGN_LINE_LEN_BEFORE_HAB#),KHAB(9,#NB_STA_ALL_HAB#),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES(#SIGN_LINE_LEN_AFTER_HAB#),CHARIOT(2)
            // INTEGER * 1 KHA2(9, NBSTA),KHS2(9, NBSTA),KHD2(9, NBSTA)
            int[,] KHA2 = new int[9 + 1, NBSTA + 1];
            int[,] KHS2 = new int[9 + 1, NBSTA + 1];
            int[,] KHD2 = new int[9 + 1, NBSTA + 1];

            // => EN PARAMETRE
            // INTEGER * 2 TABRH(NBSTA)
            // C Attention pour les stations avec Notoriété il faut metre les données total radio(#TOTAL_RADIO_INDEX#)
            // int[] TABRH = new int[NBSTA + 1]; 
            // int[] TABRH /#HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO#/

            // INTEGER*1 int KHAB(9,NB_STA_ALL_HAB),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES(#SIGN_LINE_LEN_AFTER_HAB#), CHARIOT(2)
            int[] AVANT = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
            int[,] KHAB = new int[9 + 1, NB_STA_ALL_HAB + 1];
            int[,] KHSA = new int[9 + 1, NB_STA_ALL_HAB + 1];
            int[,] KHDI = new int[9 + 1, NB_STA_ALL_HAB + 1];
            int[] APRES = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
            int[] CHARIOT = new int[2];

            // INITIALISATIONS
            int G = 0;

            // OUVERTURE FICHIERS
            Console.WriteLine("NBSTA=" + NBSTA);
            Console.WriteLine("SIGN_LINE_LEN_BEFORE_HAB=" + SIGN_LINE_LEN_BEFORE_HAB);
            Console.WriteLine("NB_STA_ALL_HAB=" + NB_STA_ALL_HAB);

            //
            //                              OUVERTURE FICHIER
            //
            // OPEN(13, FILE = '#SIGJFC_BDE#', -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')
            FileStream fs = File.Open(@"C:\AffinageART\France\Source\SFR04\U120\sig20jfc.bde", FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            int IG = 0;
            List<Fushab09Indiv> fushab09Indivs = new List<Fushab09Indiv>();
            while (fs.Position != fs.Length)
            {
                // BOUCLE INDIVIDUS
                // 30 READ(13, END = 120) AVANT,((KHAB(I, J), I = 1, 9), J = 1,#NB_STA_ALL_HAB#),((KHSA(I,J),I=1,9),J=1,#NB_STA_ALL_HAB#),
                // -((KHDI(I,J),I=1,9),J=1,#NB_STA_ALL_HAB#),APRES,CHARIOT

                for (int i = 1; i <= SIGN_LINE_LEN_BEFORE_HAB; i++)
                    AVANT[i] = br.ReadByte();

                for (int i = 1; i <= 9; i++)
                    for (int j = 1; j <= NB_STA_ALL_HAB; j++)
                        KHAB[i, j] = br.ReadByte();

                for (int i = 1; i <= 9; i++)
                    for (int j = 1; j <= NB_STA_ALL_HAB; j++)
                        KHSA[i, j] = br.ReadByte();

                for (int i = 1; i <= 9; i++)
                    for (int j = 1; j <= NB_STA_ALL_HAB; j++)
                        KHDI[i, j] = br.ReadByte();

                for (int i = 1; i <= SIGN_LINE_LEN_BEFORE_HAB; i++)
                    APRES[i] = br.ReadByte();

                for (int i = 1; i <= 2; i++)
                    CHARIOT[i] = br.ReadByte();

                IG = IG + 1;

                // BOUCLE STATIONS

                for (int NOP = 1; NOP <= NBSTA; NOP++)
                {
                    int IP = TABRH[NOP];
                    for (int I = 1; I <= 9; I++)
                    {
                        KHA2[I, NOP] = KHAB[I, IP];
                        KHS2[I, NOP] = KHSA[I, IP];
                        KHD2[I, NOP] = KHDI[I, IP];
                    }
                }
                fushab09Indivs.Add(new Fushab09Indiv(SIGN_LINE_LEN_BEFORE_HAB, NB_STA_ALL_HAB)
                {
                    AVANT = AVANT,
                    KHAB = KHAB,
                    KHSA = KHSA,
                    KHDI = KHDI,
                    APRES = APRES,
                    CHARIOT = CHARIOT,
                });
            }

            // FIN DE FICHIER
            Console.WriteLine("NB.GUS=", IG);

            return fushab09Indivs;
        }

        public struct Fushab09Indiv
        {
            public int[] AVANT;
            public int[,] KHAB;
            public int[,] KHSA;
            public int[,] KHDI;
            public int[] APRES;
            public int[] CHARIOT;

            public Fushab09Indiv(int SIGN_LINE_LEN_BEFORE_HAB, int NB_STA_ALL_HAB)
            {
                AVANT = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
                KHAB = new int[9 + 1, NB_STA_ALL_HAB + 1];
                KHSA = new int[9 + 1, NB_STA_ALL_HAB + 1];
                KHDI = new int[9 + 1, NB_STA_ALL_HAB + 1];
                APRES = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
                CHARIOT = new int[2];
            }
        }
    }
}

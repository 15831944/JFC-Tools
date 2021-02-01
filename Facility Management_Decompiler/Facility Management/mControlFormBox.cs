using Facility_Management.My;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{

    internal sealed class mControlFormBox
    {
        public static void SelectedPatch(string str = null)
        {
            bool flag = Operators.CompareString(str, null, TextCompare: false) == 0;
            if (flag)
            {
                str = Conversions.ToString(MyProject.Forms.FormBox.ListPatch.SelectedItem);
            }
            if (Operators.CompareString(str, null, TextCompare: false) != 0 && MyProject.Forms.FormBox.DicExes.ContainsKey(str))
            {
                MyProject.Forms.FormBox.DicExes[str] = false;
                MyProject.Forms.FormBox.ListSelectPatch.Items.Add(str);
                if (flag)
                {
                    MyProject.Forms.FormBox.ListPatch.Items.RemoveAt(MyProject.Forms.FormBox.ListPatch.SelectedIndex);
                }
                else
                {
                    MyProject.Forms.FormBox.ListPatch.Items.Remove(str);
                }
                MyProject.Forms.FormBox.Label4.Text = "Updates List (" + Conversions.ToString(MyProject.Forms.FormBox.ListPatch.Items.Count) + "):";
                MyProject.Forms.FormBox.Label3.Text = "Selected Updates (" + Conversions.ToString(MyProject.Forms.FormBox.ListSelectPatch.Items.Count) + "):";
            }
        }

        public static void UnSelectedPatch()
        {
            string text = Conversions.ToString(MyProject.Forms.FormBox.ListSelectPatch.SelectedItem);
            if (Operators.CompareString(text, null, TextCompare: false) == 0)
            {
                return;
            }
            MyProject.Forms.FormBox.ListSelectPatch.Items.RemoveAt(MyProject.Forms.FormBox.ListSelectPatch.SelectedIndex);
            int num = 0;
            foreach (string key in MyProject.Forms.FormBox.DicExes.Keys)
            {
                if (MyProject.Forms.FormBox.DicExes[key])
                {
                    num = checked(num + 1);
                }
                if (Operators.CompareString(key, text, TextCompare: false) == 0)
                {
                    MyProject.Forms.FormBox.DicExes[key] = true;
                    break;
                }
            }
            MyProject.Forms.FormBox.ListPatch.Items.Insert(num, text);
            MyProject.Forms.FormBox.Label4.Text = "Updates List (" + Conversions.ToString(MyProject.Forms.FormBox.ListPatch.Items.Count) + "):";
            MyProject.Forms.FormBox.Label3.Text = "Selected Updates (" + Conversions.ToString(MyProject.Forms.FormBox.ListSelectPatch.Items.Count) + "):";
        }

        public static void SelectedClient(string str = null)
        {
            bool flag = Operators.CompareString(str, null, TextCompare: false) == 0;
            if (flag)
            {
                str = Conversions.ToString(MyProject.Forms.FormBox.ListAccesClient.SelectedItem);
            }
            if (Operators.CompareString(str, null, TextCompare: false) == 0)
            {
                return;
            }
            if (MyProject.Forms.FormBox.ListAccesClient.Items.Contains(str))
            {
                MyProject.Forms.FormBox.ListSelectClient.Items.Add(str);
                if (flag)
                {
                    MyProject.Forms.FormBox.ListAccesClient.Items.RemoveAt(MyProject.Forms.FormBox.ListAccesClient.SelectedIndex);
                }
                else
                {
                    MyProject.Forms.FormBox.ListAccesClient.Items.Remove(str);
                }
            }
            MyProject.Forms.FormBox.Label1.Text = "Customer List (" + Conversions.ToString(MyProject.Forms.FormBox.ListAccesClient.Items.Count) + "):";
            MyProject.Forms.FormBox.Label2.Text = "Selected customers (" + Conversions.ToString(MyProject.Forms.FormBox.ListSelectClient.Items.Count) + "):";
        }

        public static void ConvAllClients(ListBox listclient)
        {
            MyProject.Forms.FormBox.ToolStripProgressBar1.Maximum = listclient.Items.Count;
            MyProject.Forms.FormBox.ToolStripProgressBar1.Value = 0;
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = listclient.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    ConvClient(Conversions.ToString(RuntimeHelpers.GetObjectValue(enumerator.Current)));
                    MyProject.Forms.FormBox.ToolStripProgressBar1.Value = checked(MyProject.Forms.FormBox.ToolStripProgressBar1.Value + 1);
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            MyProject.Forms.FormBox.ToolStripProgressBar1.Value = 0;
        }

        public static void ConvClient(string str = "")
        {
            Application.DoEvents();
            if (Operators.CompareString(str, "", TextCompare: false) == 0)
            {
                str = MyProject.Forms.FormBox.AliasDic[Conversions.ToString(MyProject.Forms.FormBox.ListAccesClient.SelectedItem)];
            }
            Dictionary<string, string> dictionary = new Dictionary<string, string>();
            if (Operators.CompareString(str, null, TextCompare: false) == 0)
            {
                return;
            }
            dictionary.Add("agrï¿½gat", "agrégat");
            dictionary.Add("Mise ï¿½ jour", "Mise à jour");
            dictionary.Add("Clï¿½", "Clé");
            dictionary.Add("annï¿½e", "année");
            dictionary.Add("ublicitï¿½", "ublicitïé");
            dictionary.Add("Mï¿½diamï¿½trie", "Médiamétrie");
            dictionary.Add("bloquï¿½s", "bloqués");
            dictionary.Add("ï¿½cran", "écran");
            dictionary.Add("pï¿½riode", "période");
            dictionary.Add("Lagardï¿½re", "Lagardère");
            dictionary.Add("Mï¿½tropoles", "Métropoles");
            dictionary.Add("projetï¿½s", "projetés");
            dictionary.Add("Rï¿½ouverture", "Réouverture");
            dictionary.Add("Crï¿½ation", "Création");
            dictionary.Add("associï¿½", "associé");
            dictionary.Add("libellï¿½", "libellé");
            dictionary.Add("rï¿½gle", "règle");
            dictionary.Add("Sept-Dï¿½c", "Sept-Déc");
            dictionary.Add("Indï¿½", "Indé");
            dictionary.Add("1ï¿½", "1€");
            dictionary.Add("ï¿½tudes", "études");
            dictionary.Add("60ï¿½ï¿½", "60''");
            dictionary.Add("ï¿½gie", "égie");
            dictionary.Add("Opï¿½rations", "Opérations");
            dictionary.Add("Spï¿½ciales", "Spéciales");
            dictionary.Add("prï¿½dï¿½finies", "prédéfinies");
            dictionary.Add("systÃ\u00a8me", "système");
            dictionary.Add(" Ã\u00a0 ", " à ");
            dictionary.Add(" ï¿½ ", " à ");
            dictionary.Add("spÃ©cifique", "spécifique");
            dictionary.Add("specifique", "spécifique");
            dictionary.Add("sp?cifique", "spécifique");
            dictionary.Add("spï¿½cifique", "spécifique");
            dictionary.Add("rï¿½seau", "réseau");
            dictionary.Add("rÃ©seau", "réseau");
            dictionary.Add("r?seau", "réseau");
            dictionary.Add("reseau", "réseau");
            dictionary.Add("Dï¿½cembre", "Décembre");
            dictionary.Add("Decembre", "Décembre");
            dictionary.Add("D?cembre", "Décembre");
            dictionary.Add("DÃ©cembre", "Décembre");
            dictionary.Add("NNRDécembre", "NNRDecembre");
            dictionary.Add("AoÃ»t", "Août");
            dictionary.Add("Aoï¿½t", "Août");
            dictionary.Add("Ao?t", "Août");
            dictionary.Add("Aout", "Août");
            dictionary.Add("NNRAoût", "NNRAout");
            dictionary.Add("FÃ©vrier", "Février");
            dictionary.Add("Fï¿½vrier", "Février");
            dictionary.Add("Fevrier", "Février");
            dictionary.Add("F?vrier", "Février");
            dictionary.Add("NNRFévrier", "NNRFevrier");
            dictionary.Add("PopNatFévrier", "PopNatFevrier");
            dictionary.Add("onnï¿½e", "onnée");
            dictionary.Add("onnÃ©e", "onnée");
            dictionary.Add("onn?e", "onnée");
            dictionary.Add("onnee ", "onnée");
            dictionary.Add("Ã©dia", "édia");
            dictionary.Add("?dia", "édia");
            dictionary.Add("ï¿½dia", "édia");
            dictionary.Add("?Version", "Version");
            string path = MyProject.Forms.FormBox.mPathAcces + str + MyProject.Forms.FormBox.mExtFileAcces;
            string[] array = File.ReadAllLines(path, Encoding.Default);
            checked
            {
                foreach (string key in dictionary.Keys)
                {
                    int num = array.Length - 1;
                    for (int i = 0; i <= num; i++)
                    {
                        int num2 = Strings.InStr(array[i], ".exe,", CompareMethod.Text);
                        if (num2 > 0)
                        {
                            array[i] = Strings.Mid(array[i], 1, num2 - 1) + Strings.Replace(array[i], key, dictionary[key], num2);
                        }
                    }
                }
                File.Delete(path);
                File.WriteAllLines(path, array, Encoding.Default);
            }
        }

        public static void UnSelectedClient()
        {
            string text = Conversions.ToString(MyProject.Forms.FormBox.ListSelectClient.SelectedItem);
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                MyProject.Forms.FormBox.ListAccesClient.Items.Add(text);
                MyProject.Forms.FormBox.ListSelectClient.Items.RemoveAt(MyProject.Forms.FormBox.ListSelectClient.SelectedIndex);
                MyProject.Forms.FormBox.Label1.Text = "Customer List (" + Conversions.ToString(MyProject.Forms.FormBox.ListAccesClient.Items.Count) + "):";
                MyProject.Forms.FormBox.Label2.Text = "Selected customers (" + Conversions.ToString(MyProject.Forms.FormBox.ListSelectClient.Items.Count) + "):";
            }
        }

        public static void UnSelectedAllClient()
        {
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = MyProject.Forms.FormBox.ListSelectClient.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    MyProject.Forms.FormBox.ListAccesClient.Items.Add(RuntimeHelpers.GetObjectValue(objectValue));
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            MyProject.Forms.FormBox.ListSelectClient.Items.Clear();
        }

        public static void SelectedAllClient()
        {
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = MyProject.Forms.FormBox.ListAccesClient.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    MyProject.Forms.FormBox.ListSelectClient.Items.Add(RuntimeHelpers.GetObjectValue(objectValue));
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            MyProject.Forms.FormBox.ListAccesClient.Items.Clear();
        }

        public static void UnSelectedAllPatch()
        {
            MyProject.Forms.FormBox.ListPatch.Items.Clear();
            MyProject.Forms.FormBox.ListSelectPatch.Items.Clear();
            string[] array = new string[checked(MyProject.Forms.FormBox.DicExes.Keys.Count - 1 + 1)];
            MyProject.Forms.FormBox.DicExes.Keys.CopyTo(array, 0);
            string text = MyProject.Forms.FormBox.FiltrePatch.Text;
            string[] array2 = array;
            foreach (string text2 in array2)
            {
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    if (Strings.InStr(text2, text, CompareMethod.Text) > 0)
                    {
                        MyProject.Forms.FormBox.ListPatch.Items.Add(text2);
                        MyProject.Forms.FormBox.DicExes[text2] = true;
                    }
                    else
                    {
                        MyProject.Forms.FormBox.DicExes[text2] = false;
                    }
                }
                else
                {
                    MyProject.Forms.FormBox.ListPatch.Items.Add(text2);
                    MyProject.Forms.FormBox.DicExes[text2] = true;
                }
            }
            MyProject.Forms.FormBox.Label4.Text = "Updates List (" + Conversions.ToString(MyProject.Forms.FormBox.ListPatch.Items.Count) + "):";
            MyProject.Forms.FormBox.Label3.Text = "Selected Updates (" + Conversions.ToString(MyProject.Forms.FormBox.ListSelectPatch.Items.Count) + "):";
        }
    }
}

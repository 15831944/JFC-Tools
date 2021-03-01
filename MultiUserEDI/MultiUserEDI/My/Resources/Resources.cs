// MultiUserEDI.My.Resources.Resources
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using System.CodeDom.Compiler;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.Resources;
using System.Runtime.CompilerServices;


namespace MultiUserEDI.My.Resources
{

    [StandardModule]
    [GeneratedCode("System.Resources.Tools.StronglyTypedResourceBuilder", "16.0.0.0")]
    [DebuggerNonUserCode]
    [CompilerGenerated]
    [HideModuleName]
    internal sealed class Resources
    {
        private static ResourceManager resourceMan;

        private static CultureInfo resourceCulture;

        [EditorBrowsable(EditorBrowsableState.Advanced)]
        internal static ResourceManager ResourceManager
        {
            get
            {
                if (object.ReferenceEquals(resourceMan, null))
                {
                    resourceMan = new ResourceManager("MultiUserEDI.Resources", typeof(Resources).Assembly);
                }
                return resourceMan;
            }
        }

        [EditorBrowsable(EditorBrowsableState.Advanced)]
        internal static CultureInfo Culture
        {
            get
            {
                return resourceCulture;
            }
            set
            {
                resourceCulture = value;
            }
        }

        internal static Bitmap appliquer_verifier_ok_oui_icone_5318_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("appliquer_verifier_ok_oui_icone_5318_32", resourceCulture));

        internal static Bitmap bouton_annuler_icone_4573_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("bouton_annuler_icone_4573_32", resourceCulture));

        internal static Bitmap fermer_gtk_icone_6139_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("fermer_gtk_icone_6139_32", resourceCulture));

        internal static Bitmap jouer_a_droite_fleche_icone_6822_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("jouer_a_droite_fleche_icone_6822_32", resourceCulture));

        internal static Bitmap porte_sortie_icone_9124_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("porte_sortie_icone_9124_32", resourceCulture));

        internal static Bitmap sortir_session_icone_6247_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("sortir_session_icone_6247_32", resourceCulture));

        internal static Bitmap telecharger_icone_4254_32 => (Bitmap)RuntimeHelpers.GetObjectValue(ResourceManager.GetObject("telecharger_icone_4254_32", resourceCulture));
    }
}
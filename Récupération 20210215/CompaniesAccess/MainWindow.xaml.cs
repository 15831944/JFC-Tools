using AggregateEditor;
using AR7Common;
using RadioEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace CompaniesAccess
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Engine m_engine;
        private IUserDataManager m_manager;

        public MainWindow()
        {
            InitializeComponent();

            AggregateEditorView.COLUMN_MIN_SIZE = 130;
            AggregateEditorView.COLUMN_MAX_SIZE = 500;

            this.ModeRightsCombo.SelectedItem = RightsMode.Societes;
        }

        private void Initialize(RightsMode _mode)
        {
            m_engine = new Engine();
            ConfigurationSystem l_simpleConfig = new ConfigurationSystem();

            this.m_manager = UserDataManagerFactory.Provide(_mode);

            // Flag pour savoir si une nouvelle modalité est possible
            this.m_aggrEditComponent.ViewModel.NewModalityAvailable = false;

            if (this.m_manager != null)
            {
                this.m_manager.ClearDimensions();

                l_simpleConfig.DimensionCreators.Add(this.m_manager.DimensionCreator);

                m_engine.ConfigurationSystem = l_simpleConfig;
                m_engine.Initialize();
                m_engine.LoadConfiguration("whatever");

                List<RangeIndexModalities> l_ranges = m_manager.Ranges;

                // On isole tous les users existants
                RangeIndexModalities l_range = new RangeIndexModalities();
                l_range.AddRangeValue(m_manager.Link.IndexesOfDimension[0], m_manager.Link.Dimensions[0].AllModalities.Keys.ToList());

                // Récupère une dimension agrégée pour le composant
                DimensionAggregated l_dimAgg = CreateAdaptedDimension(m_manager, l_ranges);

                this.m_aggrEditComponent.ViewModel.AllDimensions.Clear();

                // On dit au ViewModel qu'on veut travailler sur cette dimension agrégée là
                this.m_aggrEditComponent.ViewModel.ForceDimension(l_dimAgg);

                // ON cache la sélection des multiples dim
                this.m_aggrEditComponent.ViewModel.IsSelectorVisibility = Visibility.Collapsed;

                // Flag pour éviter de pouvoir ajouter de nouvelles modalités
                this.m_aggrEditComponent.ViewModel.CanOnlySelect = this.m_manager.CanOnlySelect;

                // Flag pour sélectionner toutes les modalités si on clique sur le header
                this.m_aggrEditComponent.ViewModel.SelectAllOnHeaderClick = this.m_manager.SelectAllOnHeaderClick;

                // Test la validitée d'un croisement
                this.m_aggrEditComponent.ViewModel.Validator = new DataUserValidator(this.m_manager);

                // Refresh avec les ranges sur lesquelles on travaille de façon verticale (les users)
                this.m_aggrEditComponent.ViewModel.RefreshSource(l_range);
            }
        }

        /// <summary>
        /// Va créer une dimension adaptée au composant
        /// </summary>
        /// <param name="l_manager"></param>
        /// <param name="l_ranges"></param>
        /// <returns></returns>
        private static DimensionAggregated CreateAdaptedDimension(IUserDataManager l_manager, List<RangeIndexModalities> l_ranges)
        {
            // Pour fonctionner correctement le composant a besoin d'une dimension aggrégée
            // Chaque modality doit représenter une company unique avec tous les users qui ont des droits dessus

            DimensionAggregated l_dimAgg = null;

            //DimensionAggregated l_dimAgg = new DimensionAggregated("temp", "temp");

            //// on parcourt donc la dimension users
            //foreach (KeyValuePair<AModalityIndex, BaseModality> i_kvp in l_manager.Link.Dimensions[1].Modalities)
            //{
            //    RangeIndexModalities l_aggregatedRange = new RangeIndexModalities();

            //    // Pour chaque user on isole son IndexModality correspondant
            //    IndexModalities idxUser = new IndexModalities();
            //    idxUser.AddValue(l_manager.Link.IndexesOfDimension[1], i_kvp.Key);

            //    // Dans cette expression on récupère toutes les modalités qui ont des droits pour le user d'index: idxUser
            //    RangeIndexModalities l_modalities = RangeIndexModalities.Merge(
            //        l_ranges
            //        .Where(r => r.Contains(idxUser))
            //        .Select(r => r.SubRange(new int[] { l_manager.Link.IndexesOfDimension[0] })));

            //    l_aggregatedRange.AddValue(idxUser);
            //    l_aggregatedRange.AddValue(l_modalities);

            //    // puis on crée une modality avec tous ses users
            //    AggregatedModality aggr = new AggregatedModality(i_kvp.Value.Caption, l_aggregatedRange);
            //    l_dimAgg.AddModality(aggr, true);
            //}

            //DimensionManager.Singleton.RegisterDimension(l_dimAgg);

            return l_dimAgg;
        }

        private void ButtonSave_Click(object sender, RoutedEventArgs e)
        {
            DimensionAggregated l_dimAggr = this.m_aggrEditComponent.ViewModel.CurrentDimension?.Dimension as DimensionAggregated;
            if (l_dimAggr != null)
            {
                List<RangeIndexModalities> l_resultRanges = new List<RangeIndexModalities>();

                foreach (KeyValuePair<AModalityIndex, BaseModality> i_kvpDimUser in this.m_manager.Link.Dimensions[1].AllModalities)
                {
                    RangeIndexModalities l_rangeCompaniesPerUser = new RangeIndexModalities();

                    // Pour chaque users on isole son IndexModality correspondant
                    IndexModalities idxUser = new IndexModalities();
                    idxUser.AddValue(this.m_manager.Link.IndexesOfDimension[1], i_kvpDimUser.Key);

                    // Dans cette expression on récupère tous les users qui ont des droits sur la company d'index: idxCompany
                    RangeIndexModalities l_companies = RangeIndexModalities.Merge(
                        l_dimAggr.AllModalities.Values
                        .OfType<AggregatedModality>()
                        .Where(r => r.Modalities.Contains(idxUser))
                        .Select(r => r.Modalities.SubRange(new int[] { m_manager.Link.IndexesOfDimension[0] })));

                    l_rangeCompaniesPerUser.AddValue(idxUser); // Ajoute 1 user
                    l_rangeCompaniesPerUser.AddValue(l_companies); // Ajoute toutes ses companies
                    l_resultRanges.Add(l_rangeCompaniesPerUser);
                }

                this.m_manager.SaveRanges(l_resultRanges);

                // ALAIN MESSAGE TODO
                MessageBox.Show("Enregistré avec succès.");
            }
        }

        private void OnModeChanged(object sender, SelectionChangedEventArgs e)
        {
            this.Initialize((RightsMode)this.ModeRightsCombo.SelectedItem);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RadioEngine;
using ARDataProvider;
using AR7Common;

namespace CompaniesAccess
{
    public class CompaniesManager : IUserDataManager
    {
        public CompaniesFileData Files { get; private set; }
        public DimensionLinked Link { get; private set; }

        public CompaniesManager()
        {
            this.Files = new CompaniesFileData();
            this.Link = new DimensionLinked(new String[] { CompanyUsersDimensionCreator.CompanyNames[0], CompanyUsersDimensionCreator.CompanyNames[1] });
        }

        public ADimensionCreator DimensionCreator
        {
            get { return new CompanyUsersDimensionCreator(this.Files); }
        }

        public List<RangeIndexModalities> Ranges { get { return this.GetRangesFromFiles(); } }

        public bool CanOnlySelect
        {
            get { return true; }
        }

        public bool SelectAllOnHeaderClick
        {
            get { return false; }
        }

        public bool NewModalityAvailable
        {
            get { return true; }
        }

        private List<RangeIndexModalities> GetRangesFromFiles()
        {
            List<RangeIndexModalities> l_list = new List<RangeIndexModalities>();
            this.Link.Initialize();
            if (this.Link.IsInitialized)
            {
                foreach (KeyValuePair<AModalityIndex, BaseModality> i_userModalities in this.Link.Dimensions[1].AllModalities)
                {
                    String l_currentUser = i_userModalities.Value.Caption;
                    if (this.Files.Files.ContainsKey(l_currentUser))
                    {
                        ARUserFileTable l_fileForUser = this.Files.Files[l_currentUser];

                        RangeIndexModalities l_currentRangeUser = new RangeIndexModalities();
                        l_currentRangeUser.AddValue(this.Link.IndexesOfDimension[1], i_userModalities.Key);

                        List<AModalityIndex> l_currentCompanies = new List<AModalityIndex>();
                      

                        foreach (KeyValuePair<string, ARUserFileLine> i_line in l_fileForUser.Lines)
                        {
                            if (i_line.Value.isActive)
                            {
                                AModalityIndex l_indexCompany = this.Link.GetModalityIndexOfLinkedDimension(0, i_line.Value.Company.ToLower());
                                l_currentCompanies.Add(l_indexCompany);
                            }
                        }
                        l_currentRangeUser.AddRangeValue(this.Link.IndexesOfDimension[0], l_currentCompanies);

                        l_list.Add(l_currentRangeUser);
                    }
                }
            }

            return l_list;
        }

        public bool IsValidForUser(IndexModalities _indexes)
        {
            bool l_success = false;

            Dictionary<int,BaseModality> l_modalities = this.Link.GetModalitiesOfLinkedDimension(_indexes);

            if (l_modalities.Count == 2)
            {
                String l_userName = l_modalities[this.Link.IndexesOfDimension[1]].Caption;
                if (this.Files.Files.ContainsKey(l_userName))
                {

                    ARUserFileTable l_fileForUser = this.Files.Files[l_userName];
                    String l_companyName = l_modalities[this.Link.IndexesOfDimension[0]].Caption;

                    //lines are indexed by company names
                    l_success = l_fileForUser.Lines.ContainsKey(l_companyName);

                }
            }
            return l_success;
        } 



        public void SaveRanges(List<RangeIndexModalities> _ranges)
        {
            if (this.Link.IsInitialized)
            {

                foreach (RangeIndexModalities i_range in _ranges)
                {
                    if (i_range.RangeIndexes.ContainsKey(this.Link.IndexesOfDimension[1]))
                    {
                        AModalityIndex l_userIndex = i_range.RangeIndexes[this.Link.IndexesOfDimension[1]].FirstOrDefault();
                        if (l_userIndex != null)
                        {
                            String l_user = this.Link.Dimensions[1].AllModalities[l_userIndex].Caption;

                            if (this.Files.Files.ContainsKey(l_user))
                            {
                                IEnumerable<AModalityIndex> l_indexes = null;
                                if (i_range.RangeIndexes.ContainsKey(this.Link.IndexesOfDimension[0]))
                                {
                                    l_indexes = i_range.RangeIndexes[this.Link.IndexesOfDimension[0]];

                                }
                                ARUserFileTable l_fileForUser = this.Files.Files[l_user];
                                foreach (KeyValuePair<string, ARUserFileLine> i_line in l_fileForUser.Lines)
                                {
                                    i_line.Value.isActive = false;
                                    AModalityIndex l_index = this.Link.GetModalityIndexOfLinkedDimension(0, i_line.Value.Company.ToLower());
                                    if (l_index != Dimension.EMPTY_MODALITY_INDEX)
                                    {

                                        if (l_indexes != null && l_indexes.Contains(l_index))
                                        {
                                            i_line.Value.isActive = true;
                                        }


                                    }


                                }
                                l_fileForUser.Save();



                            }

                        }


                    }


                }


            }


        }

        public void ClearDimensions()
        {
            DimensionManager.Singleton.RemoveDimension("temp");
            DimensionManager.Singleton.RemoveDimension(CompanyUsersDimensionCreator.CompanyNames[0]);
            DimensionManager.Singleton.RemoveDimension(CompanyUsersDimensionCreator.CompanyNames[1]);
        }
    }
}
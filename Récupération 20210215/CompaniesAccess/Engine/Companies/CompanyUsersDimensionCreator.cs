using AR7Common;
using ARDataProvider;
using RadioEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CompaniesAccess
{
    public class CompanyUsersDimensionCreator : ADimensionCreator
    {

        static public String[] CompanyNames= { "company", "user"};

        private  CompaniesFileData Files { get; set; }

        public CompanyUsersDimensionCreator(CompaniesFileData _files)
        {
            this.Files = _files;
        }

        protected override void CreateDimension()
        {
            this.Files.Initialize();
            Dimension l_dimCompany = new Dimension(CompanyUsersDimensionCreator.CompanyNames[0], "Nomdim");  // A TRADUIRE DIMENSION
            Dimension l_dimUser = new Dimension(CompanyUsersDimensionCreator.CompanyNames[1], "Nomdim");  // A TRADUIRE DIMENSION
            foreach (KeyValuePair<String, ARUserFileTable> i_userFile in  this.Files.Files)
            {
                BaseModality l_currentUser= new BaseModality(i_userFile.Key, Dimension.EMPTY_VALUE);
                l_dimUser.AddModality(l_currentUser);

                foreach (KeyValuePair<String, ARUserFileLine> l_line in i_userFile.Value.Lines)
                {
                    BaseModality l_currentCompany = new BaseModality(l_line.Value.Company.ToLower(), Dimension.EMPTY_VALUE);
                    l_dimCompany.AddModality(l_currentCompany);
                }
            }


            this.Dimensions.Add(l_dimCompany);
            this.Dimensions.Add(l_dimUser);



        }
    }
}

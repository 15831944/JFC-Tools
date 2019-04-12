using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AR_reconstitution
{


    public interface ITypeConversion 
    {
        String TypeElts { get; }

        String Relation { get; }

        bool IsConserned(String path);
    }

    public  class TypeConversion<T> : ITypeConversion where T :class
    {
        public String TypeElts { get; protected set; }

        public String Relation { get; protected set; }

        public TypeConversion(String type,String relation)
        {
            this.TypeElts = type;
            this.Relation = relation;
        }

        public bool IsConserned(String path)
        {
            try
            {
                StreamReader l_reader = new StreamReader(path);
                XmlSerializer SerializerObj = new XmlSerializer(typeof(T));
                T eltRts = (T)SerializerObj.Deserialize(l_reader);
                if (eltRts == null)
                    return false;

                this.FillInfo(eltRts);
                return true;
            }
            catch (Exception ex)
            {

                return false;
            }
        }

        protected virtual void FillInfo(T objectConcrete)
        {



        }

    }



    public class TypeConversionPlan: TypeConversion<PlansPlan>
    {
        public String TypeElts { get; protected set; }

        public String Relation { get; protected set; }

        public TypeConversionPlan() :base("","")
        {
        
        }

       
        protected virtual void FillInfo(PlansPlan objectConcrete)
        {
            if (objectConcrete.NrPlan > 100)
                this.TypeElts = "edi";
            else
            {
                this.TypeElts = "plan";
            }


            String lnamePlan = AtelierHelper.GetNameFilePlanXml(0, objectConcrete.Nom, objectConcrete.NrPlan, objectConcrete.Occupe);
            this.Relation = AtelierHelper.NameRlt(lnamePlan);
        }

    }

}

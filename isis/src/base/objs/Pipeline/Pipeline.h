#ifndef Pipeline_h
#define Pipeline_h
/**
 * @file
 * $Revision: 1.4 $
 * $Date: 2008/12/19 21:13:06 $
 *
 *   Unless noted otherwise, the portions of Isis written by the USGS are
 *   public domain. See individual third-party library and package descriptions
 *   for intellectual property information, user agreements, and related
 *   information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or
 *   implied, is made by the USGS as to the accuracy and functioning of such
 *   software and related material nor shall the fact of distribution
 *   constitute any such warranty, and no responsibility is assumed by the
 *   USGS in connection therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html
 *   in a browser or see the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */

#include <vector>

#include "iString.h"
#include "PipelineApplication.h"

using namespace std;

namespace Isis {
  class Filename;

  /**
   * This class helps to call other Isis Applications in a Pipeline. This object 
   * works by creating a Pipeline and setting the initial input, final
   * output of the Pipeline which are got from the user interface. Applications 
   * are added to the Pipeline and parameters relevant to the application are set 
   * using relevant APIs'. 
   *  
   * The Pipeline will control the flow of calls to the different applications in 
   * the Pipeline in First In First Out fashion. Also the first and last application 
   * in the Pipeline can be explicitly set. Pipeline automatically calculates the 
   * input to an application from the output of the previous application. 
   *  
   * The Pipeline branches automatically for a list file with multiple input files 
   * or user can explicitly create branches from the Pipeline directly or from any 
   * application in the Pipeline. The branches are required when an application 
   * generates multiple outputs. Branches may be disabled / enabled and the Pipeline 
   * will figure out the input for an application whose previous app in the same 
   * branch was disabled. 
   *  
   * Parameters for an application in a branch can be set commonly for all branches 
   * or explicitly for a branch by specifiying the branch name. 
   *  
   * Temporary files are created and will be deleted when explicitly set by the user. 
   *  
   * The Pipeline calls cubeatt app inherently if virtual bands are true. 
   *  
   * It is suggested that you "cout" this object in order to debug you're usage of 
   * the class.
   *  
   * Refer to Applications thmproc, mocproc, hidestripe for Pipeline usage. 
   *  
   * Here's an example usage of this class:
   * @code
   * UserInterface &ui = Application::GetUserInterface();
   * Pipeline p("YourAppName");
   *
   * p.SetInputFile("FROM", "BANDS");
   * p.SetOutputFile("TO");
   *
   * p.KeepTemporaryFiles(!ui.GetBoolean("REMOVE"));
   *  
   * // The app "thm2isis" generates multiple outputs for a single input. 
   * // Hence the branches have to be created to process the outputs odd, even.
   * p.AddToPipeline("thm2isis");
   * p.Application("thm2isis").SetInputParameter("FROM", false);
   * p.Application("thm2isis").SetOutputParameter("TO", "raw");
   * p.Application("thm2isis").AddBranch("even", PipelineApplication::ConstantStrings);
   * p.Application("thm2isis").AddBranch("odd", PipelineApplication::ConstantStrings);

   * p.AddToPipeline("spiceinit");
   * p.Application("spiceinit").SetInputParameter("FROM", false);
   * p.Application("spiceinit").AddParameter("PCK", "PCK");
   * p.Application("spiceinit").AddParameter("CK", "CK");
   *
   * p.AddToPipeline("cam2map");
   * p.Application("cam2map").SetInputParameter("FROM", true);
   * p.Application("cam2map").SetOutputParameter("TO", "lev2");
   * p.Application("cam2map").AddParameter("even", "MAP", "MAP");
   * p.Application("cam2map").AddParameter("even", "PIXRES", "RESOLUTION");

   * if(ui.WasEntered("PIXRES")) {
   *   p.Application("cam2map").AddConstParameter("even", "PIXRES", "MPP");
   * }

   * p.Application("cam2map").AddParameter("odd", "MAP", PipelineApplication::LastOutput);
   * p.Application("cam2map").AddConstParameter("odd", "PIXRES", "MAP");
   * p.Application("cam2map").AddConstParameter("odd", "DEFAULTRANGE", "MAP");

   * if(ui.WasEntered("PIXRES")) {
   *   p.Application("cam2map").AddConstParameter("PIXRES", "MPP");
   * }

   * if(ui.GetBoolean("INGESTION")) {
   *   p.SetFirstApplication("thm2isis");
   * }
   * else{
   *   p.SetFirstApplication("spiceinit");
   * }

   * if(ui.GetBoolean("MAPPING")) {
   *   p.SetLastApplication("cam2map");
   * }
   * else {
   *   p.SetLastApplication("spiceinit");
   * }
   *
   * p.Run();
   *
   * @endcode
   *
   * @author 2008-08-04 Steven Lambright
   *
   * @internal
   *   @history 2008-09-25 Added features: Application identifiers other than the
   *            application names, branched original input, branching from
   *            branches, partial branch merging (discontinuing branches*)
   *   @history 2008-10-28 The input no longer has to have virtual bands if the
   *            SetInputFile(iString,iString) has an empty parameter name for the
   *            virtual bands parameter. SetInputListFile(...) method added.
   *   @history 2008-12-19 List files are now fully supported, along with output
   *            list files.
   *   @history 2010-12-20 Sharmila Prasad - Added ability to add branches right off
   *            of the Pipeline
   *   @history 2010-12-21 Sharmila Prasad - Added documentation and ignore temp files
   *            from disabled branches
   */
  class Pipeline {
    public:
      Pipeline(const iString &procAppName = "");
      ~Pipeline();

      void Prepare();
      void Run();

      void SetInputFile(const iString &inputParam);
      void SetInputFile(const iString &inputParam, const iString &virtualBandsParam);
      void SetInputListFile(const iString &inputParam);
      void SetInputFile(const Filename &inputFilename);
      void SetInputListFile(const Filename &inputFilename);

      void SetOutputFile(const iString &outputParam);
      void SetOutputFile(const Filename &outputFile);
      void SetOutputListFile(const iString &outputFilenameParam);
      void SetOutputListFile(const Filename &outputFilenameList);
      void KeepTemporaryFiles(bool keep);
      //! Returns true if temporary files will not be deleted, false if they will
      bool KeepTemporaryFiles() {
        return p_keepTemporary;
      }

      void AddToPipeline(const iString &appname);
      void AddToPipeline(const iString &appname, const iString &identifier);
      PipelineApplication &Application(const iString &identifier);
      PipelineApplication &Application(const int &index);

      void SetFirstApplication(const iString &appname);
      void SetLastApplication(const iString &appname);

      friend ostream &operator<<(ostream &os, Pipeline &pipeline);

      //! Returns the name of the pipeline
      iString Name() const {
        return p_procAppName;
      }
      //! Returns the number of applications in the pipeline
      int Size() const {
        return (int)p_apps.size();
      }

      /**
       * Returns the initial input file for the pipeline
       *
       * @param branch Branch of the original input to get the filename from
       *
       * @return iString Name of the original input file
       */
      iString OriginalInput(unsigned int branch) {
        return ((branch < p_originalInput.size()) ? p_originalInput[branch] : "");
      }
      
      //! Returns the number of input files
      int OriginalInputSize() {
         return p_originalInput.size();
      }
      
      /**
       * Returns the total number of input branches 
       * Original branches = Number of input files * Number of branches
       * 
       * @author Sharmila Prasad (12/20/2010)
       * 
       * @return int - Total number of branches
       */
      int OriginalBranchesSize() {
        if (p_originalBranches.size() > 0){
          return p_originalBranches.size();
        }
        return p_inputBranches.size();
      }
      
      //! Returns the names of the original branches of the pipeline 
      //! (input files * branches if any)
      vector<iString> OriginalBranches() {
        if (p_originalBranches.size() > 0){
          return p_originalBranches;
        }
        return p_inputBranches;
      }
      
      iString FinalOutput(int branch = 0, bool addModifiers = true);
      iString TemporaryFolder();

      void EnableAllApplications();
      
      /**
       * Start off the branches directly from the pipeline
       * 
       * @author Sharmila Prasad (12/20/2010)
       * 
       * @param branch - Branch name to be added
       */
      void AddOriginalBranch(iString branch){
        int size = (int)p_inputBranches.size();
        if (size == 1) {
          p_originalBranches.push_back(branch);
        }
        else {
          for (int i=0; i<size; i++) {
            p_originalBranches.push_back(p_inputBranches[i] + "." + branch);
          }
        }
      }

    private:
      iString p_procAppName; //!< The name of the pipeline
      vector<iString> p_originalInput; //!< The original input file
      vector<iString> p_inputBranches; //!< Branches for input list
      vector<iString> p_originalBranches; //!< The input file(s) + original branches from pipeline
      vector<iString> p_finalOutput; //!< The final output file (empty if needs calculated)
      vector<iString> p_virtualBands;//!< The virtual bands string
      bool p_keepTemporary; //!< True if keeping temporary files
      bool p_addedCubeatt; //!< True if the "cubeatt" program was added
      vector< PipelineApplication * > p_apps; //!< The pipeline applications
      vector< iString > p_appIdentifiers; //!< The strings to identify the pipeline applications
      bool p_outputListNeedsModifiers;
  };
};

#endif

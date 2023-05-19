#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include "teawREX.hpp"
#include "fbridge.cc"
#include <cstdlib>
#include <typeinfo>

std::string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
 }

struct fbridgeRunner{
    std::vector<FORTRANFPTYPE> rndHel;
    std::vector<FORTRANFPTYPE> rndCol;
    std::vector<int> selHel;
    std::vector<int> selCol;
    CppObjectInFortran *fBridge;
    const unsigned int chanId = 0;
    const int nMom = 4;
    int nWarpRemain;
    int nEvt;
    int fauxNEvt;
    int nPar;
    fbridgeRunner(){}
    fbridgeRunner( REX::lheNode& lheFile ){
        if( !lheFile.isParsed() ){ lheFile.deepParse(); }
        nEvt = lheFile.events.size();
        nWarpRemain = (32 - ( nEvt % 32 )) % 32;
        fauxNEvt = nEvt + nWarpRemain;
        rndHel = std::vector<FORTRANFPTYPE>( fauxNEvt, 0. );
        rndCol = std::vector<FORTRANFPTYPE>( fauxNEvt, 0. );
        selHel = std::vector<int>( fauxNEvt, 0 );
        selCol = std::vector<int>( fauxNEvt, 0 );
        nPar = lheFile.events[0]->getPrts().size();
    }
    fbridgeRunner( std::shared_ptr<REX::lheNode> lheFile ){
        if(!lheFile->isParsed() ){ lheFile->deepParse(); }
        nEvt = lheFile->events.size();
        nWarpRemain = (32 - ( nEvt % 32 )) % 32;
        fauxNEvt = nEvt + nWarpRemain;
        rndHel = std::vector<FORTRANFPTYPE>( fauxNEvt, 0. );
        rndCol = std::vector<FORTRANFPTYPE>( fauxNEvt, 0. );
        selHel = std::vector<int>( fauxNEvt, 0 );
        selCol = std::vector<int>( fauxNEvt, 0 );
        nPar = lheFile->events[0]->getPrts().size();
    }
#if defined MGONGPU_FPTYPE_FLOAT
    std::shared_ptr<std::vector<FORTRANFPTYPE>> scatAmp( std::shared_ptr<std::vector<float>> momenta, std::shared_ptr<std::vector<float>> alphaS ){
        auto evalScatAmps = std::make_shared<std::vector<FORTRANFPTYPE>>( fauxNEvt );
        fbridgecreate_( &fBridge, &fauxNEvt, &nPar, &nMom );
        fbridgesequence_( &fBridge, &momenta->at(0), &alphaS->at(0), &rndHel[0], &rndCol[0], &chanId, &evalScatAmps->at(0), &selHel[0], &selCol[0] );
        fbridgedelete_( &fBridge );
        evalScatAmps->resize( nEvt );
        return evalScatAmps;
    }
    std::shared_ptr<std::vector<FORTRANFPTYPE>> scatAmp( std::vector<float>& momenta, std::vector<float>& alphaS ){
        auto evalScatAmps = std::make_shared<std::vector<FORTRANFPTYPE>>( nEvt );
        fbridgecreate_( &fBridge, &fauxNEvt, &nPar, &nMom );
        fbridgesequence_( &fBridge, &momenta[0], &alphaS[0], &rndHel[0], &rndCol[0], &chanId, &evalScatAmps->at(0), &selHel[0], &selCol[0] );
        fbridgedelete_( &fBridge );
        evalScatAmps->resize( nEvt );
        return evalScatAmps;
    }
#endif
    std::shared_ptr<std::vector<FORTRANFPTYPE>> scatAmp( std::shared_ptr<std::vector<double>> momenta, std::shared_ptr<std::vector<double>> alphaS ){
        if( alphaS->size() != fauxNEvt ){
            for( int k = 0; k < nWarpRemain ; ++k ){
                alphaS->push_back( 0. );
                for( int k = 0 ; k < 4 * nPar ; ++k ){
                    momenta->push_back( 0. );
                }
            }
        }
#if defined MGONGPU_FPTYPE_FLOAT
        auto nuMom = std::make_shared<std::vector<float>>( fauxNEvt );
        auto nuAlphaS = std::make_shared<std::vector<float>>( fauxNEvt );
        std::transform( momenta->begin(), momenta->end(), nuMom->begin(), [](double mom){ return static_cast<float>(mom); });
        std::transform( alphaS->begin(), alphaS->end(), nuAlphaS->begin(), [](double gs){ return static_cast<float>(gs); });
        return scatAmp( nuMom, nuAlphaS );
#elif defined MGONGPU_FPTYPE_DOUBLE
        auto evalScatAmps = std::make_shared<std::vector<FORTRANFPTYPE>>( fauxNEvt );
        fbridgecreate_( &fBridge, &fauxNEvt, &nPar, &nMom );
        fbridgesequence_( &fBridge, &momenta->at(0), &alphaS->at(0), &rndHel[0], &rndCol[0], &chanId, &evalScatAmps->at(0), &selHel[0], &selCol[0] );
        fbridgedelete_( &fBridge );
        evalScatAmps->resize( nEvt );
        return evalScatAmps;
#endif
    }
    std::shared_ptr<std::vector<FORTRANFPTYPE>> scatAmp( std::vector<double>& momenta, std::vector<double>& alphaS ){
        if( alphaS.size() != fauxNEvt ){
            for( int k = 0; k < nWarpRemain ; ++k ){
                alphaS.push_back( 0. );
                for( int k = 0 ; k < 4 * nPar ; ++k ){
                    momenta.push_back( 0. );
                }
            }
        }
#if defined MGONGPU_FPTYPE_FLOAT
        auto nuMom = std::vector<float>( fauxNEvt );
        auto nuAlphaS = std::vector<float>( fauxNEvt );
        std::transform( momenta.begin(), momenta.end(), nuMom.begin(), [](double mom){ return static_cast<float>(mom); });
        std::transform( alphaS.begin(), alphaS.end(), nuAlphaS.begin(), [](double gs){ return static_cast<float>(gs); });
        return scatAmp( nuMom, nuAlphaS );
#elif defined MGONGPU_FPTYPE_DOUBLE
        auto evalScatAmps = std::make_shared<std::vector<FORTRANFPTYPE>>( fauxNEvt );
        fbridgecreate_( &fBridge, &fauxNEvt, &nPar, &nMom );
        fbridgesequence_( &fBridge, &momenta[0], &alphaS[0], &rndHel[0], &rndCol[0], &chanId, &evalScatAmps->at(0), &selHel[0], &selCol[0] );
        fbridgedelete_( &fBridge );
        evalScatAmps->resize( nEvt );
        return evalScatAmps;
#endif
    }
}; 

const std::string redCl = "\033[31m";
const std::string greenCl = "\033[32m";
const std::string resetCl = "\033[0m";

int usage( char* argv0, int ret = 1 )
{
    std::cout << greenCl << "\nUsage:\t " << resetCl << argv0
        <<"\t[--lhefile=\"/YOUR/PATH/HERE\"|-lhe=\"/YOUR/PATH/HERE\"]\n\t\t\t[--rwgtcard=/YOUR/PATH/HERE|-rwgt=\"/YOUR/PATH/HERE\"]\n"
        << "\t\t\t[--output=/YOUR/PATH/HERE\"|-out=\"/YOUR/PATH/HERE\"]\n";
    std::cout << "\n";
    std::cout << "\tThe LHE file path should be with respect to the directory you are running\n";
    std::cout << "\tthis program from, and similarly the rwgt_card should be as well.\n\n";
#if defined MGONGPU_FPTYPE_DOUBLE
    std::cout << "\tThe program is currently compiled with double precision.\n";
#elif defined MGONGPU_FPTYPE_FLOAT
    std::cout << "\tThe program is currently compiled with float precision.\n";
#else
    std::cout << "\tThe program is currently compiled with an unrecognised precision -- FPTYPE is defined neither as float nor double for GPU evaluations. Numerical \n";
#endif
    std::cout << "\tprecision can only be redefined at compile time. If you wish to change the precision,\n\tplease recompile with the option \""
    << redCl << "FPTYPE=f" << resetCl << "\"/\"" << redCl << "FPTYPE=d" << resetCl << "\".\n\n";
    return ret;
}

int main( int argc, char** argv ){
    std::string lheFilePath;
    std::string rwgtCardPath;
    std::string outputPath;
    std::string slhaPath;

    // READ COMMAND LINE ARGUMENTS
    for( int argn = 1; argn < argc; ++argn )
    {
        std::string currArg = argv[argn];
        if( currArg.substr(0,9) == "--lhefile" || currArg.substr(0,4) == "-lhe" )
        {
            lheFilePath = currArg.substr( currArg.find( "=" ) + 1 ); 
        }
        else if( currArg.substr(0,10) == "--rwgtcard" || currArg.substr(0,5) == "-rwgt" )
        {
            rwgtCardPath = currArg.substr( currArg.find( "=" ) + 1 );
        } else if( currArg.substr(0,8) == "--output" || currArg.substr(0,4) == "-out" ){
            outputPath = currArg.substr( currArg.find( "=" ) + 1 );
        } else
        {
            return usage( argv[0] );
        }
    }

    if( lheFilePath.empty() || rwgtCardPath.empty() ){
        return usage( argv[0] );
    }

    std::string currPath = get_current_dir();

    size_t slashPos = currPath.find_last_of( "/" ); 
    bool onWindows = false;
    if( slashPos == std::string::npos ){ slashPos = currPath.find_last_of( "\\" ); onWindows = true; }
    if( slashPos == std::string::npos )
        throw std::runtime_error( "Failed to determine current working directory -- need to know where program is run from to identify where to pull and push param_card.dat." );

    if( onWindows ){
        if( currPath.substr( slashPos + 1, 2 ) == "P1" ){
            slhaPath = "..\\..\\Cards\\param_card.dat";
        } else{
            slhaPath = "Cards\\param_card.dat";
        }
    } else {
        if( currPath.substr( slashPos + 1, 2 ) == "P1" ){
            slhaPath = "../../Cards/param_card.dat";
        } else {
            slhaPath = "Cards/param_card.dat";
        }
    }
    

    REX::teaw::rwgtFiles fileCol( lheFilePath, slhaPath, rwgtCardPath );
    fileCol.initCards();


    if( fileCol.getLhe()->events[0]->getPrts().size() != mgOnGpu::npar ){
        throw std::runtime_error("Number of external particles in input LHE file differs from nimber of external particles for this process -- process mismatch.");
    }
    
    auto bridgeCont = fbridgeRunner( fileCol.getLhe() );

    std::function<std::shared_ptr<std::vector<FORTRANFPTYPE>>(std::vector<double>&, std::vector<double>&)> scatteringAmplitude =
    [&bridgeCont](std::vector<double>& momenta, std::vector<double>& alphaS) {
        return bridgeCont.scatAmp(momenta, alphaS);
    };
    REX::teaw::rwgtRunner nuRun( fileCol, scatteringAmplitude );


    nuRun.runRwgt( outputPath ); 
 
    return 0;

}

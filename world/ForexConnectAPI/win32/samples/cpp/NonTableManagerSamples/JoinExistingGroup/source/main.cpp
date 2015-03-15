#include "stdafx.h"
#include "ResponseListener.h"
#include "SessionStatusListener.h"
#include "LoginParams.h"
#include "SampleParams.h"
#include "CommonSources.h"

void printHelp(std::string &);
bool checkObligatoryParams(LoginParams *, SampleParams *);
void printSampleParams(std::string &, LoginParams *, SampleParams *);
IO2GRequest *addOrdersIntoGroupRequest(IO2GSession *, const char *, const char *,
        std::vector<std::string> &, int);
bool isOrderExists(IO2GSession *, const char *, const char *, ResponseListener *);
bool isContingencyExists(IO2GSession *, const char *, const char *, ResponseListener *);

int main(int argc, char *argv[])
{
    std::string procName = "JoinExistingGroup";
    if (argc == 1)
    {
        printHelp(procName);
        return -1;
    }

    bool bWasError = false;

    LoginParams *loginParams = new LoginParams(argc, argv);
    SampleParams *sampleParams = new SampleParams(argc, argv);

    printSampleParams(procName, loginParams, sampleParams);
    if (!checkObligatoryParams(loginParams, sampleParams))
        return -1;

    IO2GSession *session = CO2GTransport::createSession();
    int iContingencyGroupType = 1; // OCO group

    SessionStatusListener *sessionListener = new SessionStatusListener(session, true,
                                                                       loginParams->getSessionID(),
                                                                       loginParams->getPin());
    session->subscribeSessionStatus(sessionListener);

    bool bConnected = login(session, sessionListener, loginParams);

    if (bConnected)
    {
        bool bIsAccountEmpty = !sampleParams->getAccount() || strlen(sampleParams->getAccount()) == 0;
        O2G2Ptr<IO2GAccountRow> account = getAccount(session, sampleParams->getAccount());
        ResponseListener *responseListener = new ResponseListener(session);
        session->subscribeResponse(responseListener);
        if (account)
        {
            if (bIsAccountEmpty)
            {
                sampleParams->setAccount(account->getAccountID());
                std::cout << "Account: " << sampleParams->getAccount() << std::endl;
            }

            std::vector<std::string> orderIDs(1);
            orderIDs[0] = sampleParams->getOrderID();

            for (size_t i=0; i < orderIDs.size(); ++i)
            {
                if (!isOrderExists(session, sampleParams->getAccount(), orderIDs[i].c_str(), responseListener))
                {
                    std::cout << "Order '" << orderIDs[i] << "' does not exist" << std::endl;
                    bWasError = true;
                }
            }

            if (!isContingencyExists(session, sampleParams->getAccount(), sampleParams->getContingencyID(), responseListener))
            {
                std::cout << "Contingency group '" << sampleParams->getContingencyID() << "' does not exist" << std::endl;
                bWasError = true;
            }

            if (!bWasError)
            {
                O2G2Ptr<IO2GRequest> request = addOrdersIntoGroupRequest(session, sampleParams->getAccount(),
                        sampleParams->getContingencyID(), orderIDs, iContingencyGroupType);
                if (request)
                {
                    responseListener->setRequestID(request->getRequestID());
                    responseListener->setOrderIDs(orderIDs);
                    session->sendRequest(request);
                    if (responseListener->waitEvents())
                    {
                        std::cout << "Done!" << std::endl;
                    }
                    else
                    {
                        std::cout << "Response waiting timeout expired" << std::endl;
                        bWasError = true;
                    }
                }
                else
                {
                    std::cout << "Cannot create request" << std::endl;
                    bWasError = true;
                }
            }
        }
        else
        {
            std::cout << "No valid accounts" << std::endl;
            bWasError = true;
        }
        session->unsubscribeResponse(responseListener);
        responseListener->release();
        logout(session, sessionListener);
    }
    else
    {
        bWasError = true;
    }

    session->unsubscribeSessionStatus(sessionListener);
    sessionListener->release();
    session->release();

    if (bWasError)
        return -1;
    return 0;
}

IO2GRequest *addOrdersIntoGroupRequest(IO2GSession *session, const char *sAccountID, const char *sContingencyID,
        std::vector<std::string> &orderIDs, int iContingencyType)
{
    O2G2Ptr<IO2GRequestFactory> requestFactory = session->getRequestFactory();
    if (!requestFactory)
    {
        std::cout << "Cannot create request factory" << std::endl;
        return NULL;
    }
    O2G2Ptr<IO2GValueMap> valuemap = requestFactory->createValueMap();
    valuemap->setString(Command, O2G2::Commands::JoinToExistingContingencyGroup);
    valuemap->setInt(ContingencyGroupType, iContingencyType);
    valuemap->setString(ContingencyID, sContingencyID);

    for (size_t i=0; i < orderIDs.size(); ++i)
    {
        O2G2Ptr<IO2GValueMap> valuemapChild = requestFactory->createValueMap();
        valuemapChild->setString(OrderID, orderIDs[i].c_str());
        valuemapChild->setString(AccountID, sAccountID);
        valuemap->appendChild(valuemapChild);
    }

    O2G2Ptr<IO2GRequest> request = requestFactory->createOrderRequest(valuemap);
    if (!request)
    {
        std::cout << requestFactory->getLastError() << std::endl;
        return NULL;
    }
    return request.Detach();
}

bool isOrderExists(IO2GSession *session, const char *sAccountID, const char *sOrderID, ResponseListener *responseListener)
{
    if (!session || !responseListener || !sAccountID)
        return false;

    O2G2Ptr<IO2GRequestFactory> requestFactory = session->getRequestFactory();
    if (!requestFactory)
    {
        std::cout << "Cannot create request factory" << std::endl;
        return false;
    }

    O2G2Ptr<IO2GRequest> request = requestFactory->createRefreshTableRequestByAccount(Orders, sAccountID);
    responseListener->setRequestID(request->getRequestID());
    session->sendRequest(request);
    if (!responseListener->waitEvents())
    {
        std::cout << "Response waiting timeout expired" << std::endl;
        return false;
    }
    O2G2Ptr<IO2GResponse> response = responseListener->getResponse();
    if (response)
    {
        O2G2Ptr<IO2GResponseReaderFactory> readerFactory = session->getResponseReaderFactory();
        if (readerFactory)
        {
            O2G2Ptr<IO2GOrdersTableResponseReader> ordersResponseReader = readerFactory->createOrdersTableReader(response);
            for (int i = 0; i < ordersResponseReader->size(); ++i)
            {
                O2G2Ptr<IO2GOrderRow> order = ordersResponseReader->getRow(i);
                if (sOrderID && strcmp(order->getOrderID(), sOrderID) == 0)
                    return true;
            }
        }
    }
    return false;
}

bool isContingencyExists(IO2GSession *session, const char *sAccountID, const char *sContingencyID, ResponseListener *responseListener)
{
    if (!session || !responseListener || !sAccountID)
        return false;

    O2G2Ptr<IO2GRequestFactory> requestFactory = session->getRequestFactory();
    if (!requestFactory)
    {
        std::cout << "Cannot create request factory" << std::endl;
        return false;
    }

    O2G2Ptr<IO2GRequest> request = requestFactory->createRefreshTableRequestByAccount(Orders, sAccountID);
    responseListener->setRequestID(request->getRequestID());
    session->sendRequest(request);
    if (!responseListener->waitEvents())
    {
        std::cout << "Response waiting timeout expired" << std::endl;
        return false;
    }
    O2G2Ptr<IO2GResponse> response = responseListener->getResponse();
    if (response)
    {
        O2G2Ptr<IO2GResponseReaderFactory> readerFactory = session->getResponseReaderFactory();
        if (readerFactory)
        {
            O2G2Ptr<IO2GOrdersTableResponseReader> ordersResponseReader = readerFactory->createOrdersTableReader(response);
            for (int i = 0; i < ordersResponseReader->size(); ++i)
            {
                O2G2Ptr<IO2GOrderRow> order = ordersResponseReader->getRow(i);
                if (sContingencyID && strcmp(order->getContingentOrderID(), sContingencyID) == 0)
                    return true;
            }
        }
    }
    return false;
}

void printSampleParams(std::string &sProcName, LoginParams *loginParams, SampleParams *sampleParams)
{
    std::cout << "Running " << sProcName << " with arguments:" << std::endl;

    // Login (common) information
    if (loginParams)
    {
        std::cout << loginParams->getLogin() << " * "
                << loginParams->getURL() << " "
                << loginParams->getConnection() << " "
                << loginParams->getSessionID() << " "
                << loginParams->getPin() << std::endl;
    }

    // Sample specific information
    if (sampleParams)
    {
        std::cout << "ContingencyID='" << sampleParams->getContingencyID() << "', "
                << "OrderID='" << sampleParams->getOrderID() << "', "
                << "Account='" << sampleParams->getAccount() << "'"
                << std::endl;
    }
}

void printHelp(std::string &sProcName)
{
    std::cout << sProcName << " sample parameters:" << std::endl << std::endl;
            
    std::cout << "/login | --login | /l | -l" << std::endl;
    std::cout << "Your user name." << std::endl << std::endl;
                
    std::cout << "/password | --password | /p | -p" << std::endl;
    std::cout << "Your password." << std::endl << std::endl;
                
    std::cout << "/url | --url | /u | -u" << std::endl;
    std::cout << "The server URL. For example, http://www.fxcorporate.com/Hosts.jsp." << std::endl << std::endl;
                
    std::cout << "/connection | --connection | /c | -c" << std::endl;
    std::cout << "The connection name. For example, \"Demo\" or \"Real\"." << std::endl << std::endl;
                
    std::cout << "/sessionid | --sessionid " << std::endl;
    std::cout << "The database name. Required only for users who have accounts in more than one database. Optional parameter." << std::endl << std::endl;
                
    std::cout << "/pin | --pin " << std::endl;
    std::cout << "Your pin code. Required only for users who have a pin. Optional parameter." << std::endl << std::endl;
                
    std::cout << "/orderid | --orderid " << std::endl;
    std::cout << "Order, which you want to remove from existing contingency group." << std::endl << std::endl;

    std::cout << "/contingencyid | --contingencyid " << std::endl;
    std::cout << "Contingency ID of group, to which you want to join an entry order. Mandatory argument." << std::endl << std::endl;
            
    std::cout << "/account | --account " << std::endl;
    std::cout << "An account which you want to use in sample. Optional parameter." << std::endl << std::endl;
}

bool checkObligatoryParams(LoginParams *loginParams, SampleParams *sampleParams)
{
    /* Check login parameters. */
    if (strlen(loginParams->getLogin()) == 0)
    {
        std::cout << LoginParams::Strings::loginNotSpecified << std::endl;
        return false;
    }
    if (strlen(loginParams->getPassword()) == 0)
    {
        std::cout << LoginParams::Strings::passwordNotSpecified << std::endl;
        return false;
    }
    if (strlen(loginParams->getURL()) == 0)
    {
        std::cout << LoginParams::Strings::urlNotSpecified << std::endl;
        return false;
    }
    if (strlen(loginParams->getConnection()) == 0)
    {
        std::cout << LoginParams::Strings::connectionNotSpecified << std::endl;
        return false;
    }

    /* Check other parameters. */
    if (strlen(sampleParams->getOrderID()) == 0)
    {
        std::cout << SampleParams::Strings::orderidNotSpecified << std::endl;
        return false;
    }
    if (strlen(sampleParams->getContingencyID()) == 0)
    {
        std::cout << SampleParams::Strings::contingencyidNotSpecified << std::endl;
        return false;
    }

    return true;
}


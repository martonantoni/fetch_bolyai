#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <format>
#include <print> 
#include <ranges>
#include <unordered_map>

std::string downloadFolder = "./downloads";

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* out)
{
    size_t totalSize = size * nmemb;
    out->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string downloadFile(const std::string& url, const std::string& localPath) 
{
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize curl" << std::endl;
        return {};
    }
    std::string data;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) 
    {
        std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
    }
    else 
    {
        long res;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res);
        if (res != 200)
        {
            data.clear(); 
            std::print("downloading {} failed with status code {}\n", url, res);
        }
        else
        {
            // Save the content to the local file
            std::ofstream outfile(localPath);
            outfile << data;
            outfile.close();
        }
    }
    curl_easy_cleanup(curl); 
    return data;
}

void downloadFileContent(const std::string& url, const std::string& localFilePath)
{
    if (std::filesystem::exists(localFilePath))
    {
        std::print("file {} already exists, skipping download\n", localFilePath);
        return;
    }
    std::print("downloading {}...\n", url);
    std::string content = downloadFile(url, localFilePath);
    if (content.empty())
    {
        return;
    }
    // save file content to local file
    std::ofstream outfile(localFilePath, std::ios::binary);
    outfile << content;
    outfile.close();
}

int main(int argc, char* argv[]) 
{
    // make sure download folder exists:
    std::filesystem::create_directories(downloadFolder);


    for (int year = 5; year <= 24; ++year)
    {
        for(int grade = 3; grade <= 8; ++grade)
        {
            downloadFileContent(
                std::format("https://matek.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_{2:02}.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_{3}.pdf", downloadFolder, year, year + 1, grade));
            downloadFileContent(
                std::format("https://matek.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_{2:02}_donto.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_donto_{3}.pdf", downloadFolder, year, year + 1, grade));
        }
        downloadFileContent(
            std::format("https://matek.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_megoldokulcs.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_megoldas.pdf", downloadFolder, year, year + 1));
        downloadFileContent(
            std::format("https://matek.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_donto_megoldokulcs.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_donto_megoldas.pdf", downloadFolder, year, year + 1));
    }
    for (int year = 14; year <= 18; ++year)
    {
        for (int grade = 9; grade <= 12; ++grade)
        {
            downloadFileContent(
                std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/megyei{2:02}.pdf", year, year + 1, grade), 
                std::format("{0}/20{1:02}-{2:02}_megyei_{3}.pdf", downloadFolder, year, year + 1, grade));
            downloadFileContent(
                std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/donto{2:02}.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_donto_{3}.pdf", downloadFolder, year, year + 1, grade));
        }
        downloadFileContent(
            std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/megyei_megold.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_megyei_megoldas.pdf", downloadFolder, year, year + 1));
        downloadFileContent(
            std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/donto_megold.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_donto_megoldas.pdf", downloadFolder, year, year + 1));
    }
    {
        int year = 20;
        for (int grade = 9; grade <= 12; ++grade)
        {
            downloadFileContent(
                std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/{0:02}{1:02}_matek_{2:02}.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_megyei_{3}.pdf", downloadFolder, year, year + 1, grade));
            downloadFileContent(
                std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/{0:02}{1:02}_matek_{2:02}_donto.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_donto_{3}.pdf", downloadFolder, year, year + 1, grade));
        }
        downloadFileContent(
            std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/megyei_megold.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_megyei_megoldas.pdf", downloadFolder, year, year + 1));
        downloadFileContent(
            std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/donto_megold.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_donto_megoldas.pdf", downloadFolder, year, year + 1));
    }
    for (int year = 21; year <= 24; ++year)
    {
        for (int grade = 9; grade <= 12; ++grade)
        {
            downloadFileContent(
                std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_{2:02}.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_megyei_{3}.pdf", downloadFolder, year, year + 1, grade));
            downloadFileContent(
                std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_{2:02}_donto.pdf", year, year + 1, grade),
                std::format("{0}/20{1:02}-{2:02}_donto_{3}.pdf", downloadFolder, year, year + 1, grade));
        }
        downloadFileContent(
            std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_912_megoldokulcs.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_megyei_megoldas.pdf", downloadFolder, year, year + 1));
        downloadFileContent(
            std::format("https://matek912.bolyaiverseny.hu/feladatok/20{0:02}-{1:02}/20{0:02}{1:02}_matek_912_donto_megoldokulcs.pdf", year, year + 1),
            std::format("{0}/20{1:02}-{2:02}_donto_megoldas.pdf", downloadFolder, year, year + 1));
    }

    return 0;
}

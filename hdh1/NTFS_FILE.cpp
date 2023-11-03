#include "NTFS_FILE.h"

NTFS_FILE::NTFS_FILE() {
	_id = -1;
	_name = "NOTHING";
	_drive = DEFAULT_DRIVE;
	_idParent = -1;
	_status = -1;
	_size = -1;
};

NTFS_FILE::NTFS_FILE(int ID, string NAME, LPCWSTR DRIVE, int IDPARENT, int STATUS, int SIZE) {
	_id = ID;
	_name = NAME;
	_drive = DRIVE;
	_idParent = IDPARENT;
	_status = STATUS;
	_size = SIZE;
}

NTFS_FILE::~NTFS_FILE() {};


void NTFS_FILE::getFile(BYTE sectors[]) {

    if (read_offset_unicode("00", 5, sectors) != "FILE0")
    {
        return;
    }

    _id = Hex2Dec(read_offset("2C", 4, sectors));
    _name = findFileName(sectors);
    _drive = DEFAULT_DRIVE;
    // _idParent = 
    _status = Hex2Dec(read_offset("16", 2, sectors));
    // _size = 

    // To-do: thêm thuộc tính _flag, đọc trong standard_information
    string readPoint = read_offset("14", 2, sectors);
    while (true) {

    }


}

void NTFS_FILE::printFile_Info() {
	cout << "Id: " << _id << endl;
	cout << "Name: " << _name << endl;
	cout << "Size: " << _size << endl;
	cout << "Drive: " << _drive << endl;
	cout << "IdParent: " << _idParent << endl;
	cout << "Status: " << _status << endl;
}

void NTFS_FILE::printFile_Name() {
	cout << "File: " << _name << " (Id: " << _id << ") " << endl;
}

void NTFS_FILE::printFile_Data()
{
    //cout << this->findData();
    findData();
    cout << endl;
}

string NTFS_FILE::findFileName(const BYTE MFT[1024]) {
    string file_name;
    string first_attribute_offset = read_offset("14", 2, MFT);
    string temp_offset = first_attribute_offset;
    long long next_offset = 0;
    long long attribute_size = 0;
    bool emergency_break = false;
    bool MFT_dat = false;

    while (read_offset(Dec2Hex(Hex2Dec(temp_offset)), 4, MFT) != "ffffffff" && read_offset(Dec2Hex(Hex2Dec(temp_offset)), 4, MFT) != "FFFFFFFF") {
        if (emergency_break)
            break;
        string attribute = read_offset(temp_offset, 16, MFT);
        long long attribute_type_id = Hex2Dec(attribute.substr(24, 8));
        attribute_size = Hex2Dec(attribute.substr(16, 8)) % 0x400;
        bool non_resident_flag = Hex2Dec(attribute.substr(14, 2));
        long long name_length = Hex2Dec(attribute.substr(12, 2));
        string name_offset = attribute.substr(8, 4);
        string flags = attribute.substr(4, 4);
        string attribute_id = attribute.substr(0, 4);

        long long size = attribute_size - 16;
        string attribute_ext_1 = read_offset(Dec2Hex(Hex2Dec(temp_offset) + 16), size, MFT);
        long long attribute_data_length = Hex2Dec(attribute_ext_1.substr((size - 4) * 2, 8));
        string attribute_data_offset = attribute_ext_1.substr((size - 6) * 2, 4);

        // $FILE_NAME
        if (attribute_type_id == 48) {
            size = attribute_size - 24;
            string attribute_ext_2 = read_offset(Dec2Hex(Hex2Dec(temp_offset) + Hex2Dec(attribute_data_offset)), size, MFT);
            long long file_name_length = Hex2Dec(attribute_ext_2.substr((size - 65) * 2, 2));

            // This is the file name
            file_name = MFT_Name(Dec2Hex(Hex2Dec(temp_offset) + 66 + Hex2Dec(attribute_data_offset)), file_name_length * 2, MFT);
            // cout << endl;
            // cout << "File name: " << file_name << endl;
            // if (file_name[0] == '$')
            emergency_break = true;
        }

        temp_offset = Dec2Hex(Hex2Dec(temp_offset) + attribute_size);
    }
    return file_name;
}

string NTFS_FILE::findData() {
    BYTE sector[512], tempSector[512], tempMFT[1024];
    ReadSector(_drive, 0, sector, 512);
    string data;

    long long bytes_per_sector = Hex2Dec(read_offset("B", 2, sector));
    long long sectors_per_cluster = Hex2Dec(read_offset("D", 1, sector));
    long long MFT_first_cluster = Hex2Dec(read_offset("30", 8, sector));
    long long MFT_first_sector = sectors_per_cluster * MFT_first_cluster;

    // MFT Table
    ReadSector(_drive, (MFT_first_sector + _id * 2) * bytes_per_sector, tempMFT, 1024);
    string first_attribute_offset = read_offset("14", 2, tempMFT);
    string temp_offset = first_attribute_offset;
    //long long MFT_Clusters = MFT_clusters(Dec2Hex(Hex2Dec(first_attribute_offset)), drive, sector, tempMFT);
    long long record_ID = Hex2Dec(read_offset("2C", 2, tempMFT));

    if (MFT_Name("0", 4, tempMFT) == "FILE") {
        long long next_offset = 0;
        long long attribute_size = 0;
        bool emergency_break = false;
        string temp_offset = first_attribute_offset;
        while (read_offset(Dec2Hex(Hex2Dec(temp_offset)), 4, tempMFT) != "ffffffff" && read_offset(Dec2Hex(Hex2Dec(temp_offset)), 4, tempMFT) != "FFFFFFFF") {
            string attribute = read_offset(temp_offset, 16, tempMFT);
            long long attribute_type_id = Hex2Dec(attribute.substr(24, 8));
            attribute_size = Hex2Dec(attribute.substr(16, 8)) % 0x400;
            bool non_resident_flag = Hex2Dec(attribute.substr(14, 2));
            long long name_length = Hex2Dec(attribute.substr(12, 2));
            string name_offset = attribute.substr(8, 4);
            string flags = attribute.substr(4, 4);
            string attribute_id = attribute.substr(0, 4);

            long long size = attribute_size - 16;
            string attribute_ext_1 = read_offset(Dec2Hex(Hex2Dec(temp_offset) + 16), size, tempMFT);
            long long attribute_data_length = Hex2Dec(attribute_ext_1.substr((size - 4) * 2, 8));
            string attribute_data_offset = attribute_ext_1.substr((size - 6) * 2, 4);

            if (attribute_type_id == 128) {
                // This is the Data
                if (!non_resident_flag) {
                    data = MFT_Name(Dec2Hex(Hex2Dec(temp_offset) + Hex2Dec(attribute_data_offset)), attribute_data_length, tempMFT);
                    cout << endl;
                    cout << "Data: " << data << endl;
                    cout << "Size: " << dec << attribute_data_length << endl;
                }
                else {
                    size = attribute_size - 32;
                    string attribute_ext_2 = read_offset(Dec2Hex(Hex2Dec(temp_offset) + 32), size, tempMFT);
                    string data_offset = attribute_ext_2.substr((size - 2) * 2, 4);

                    size = attribute_size - Hex2Dec(data_offset);
                    string attribute_ext_3 = read_offset(Dec2Hex(Hex2Dec(temp_offset) + Hex2Dec(data_offset)), size, tempMFT);

                    long long cluster_count_parse = 0;
                    long long fragment_cluster_parse = 0;
                    long long cluster_count = 0;
                    long long fragment_cluster = 0;
                    long long total_clusters = 0;

                    cout << endl;
                    cout << "Data: ";

                    for (int i = (int)attribute_ext_3.length() - 1; i >= 0; i--) {
                        // Read fragmented data
                        cluster_count_parse = Hex2Dec(attribute_ext_3.substr(i--, 1));
                        fragment_cluster_parse = Hex2Dec(attribute_ext_3.substr(i, 1));
                        if (cluster_count_parse > 0)
                        {
                            i -= cluster_count_parse * 2;
                            cluster_count = Hex2Dec(attribute_ext_3.substr(i, cluster_count_parse * 2));
                            total_clusters += cluster_count;
                        }
                        else
                            continue;
                        if (fragment_cluster_parse > 0)
                        {
                            i -= fragment_cluster_parse * 2;
                            fragment_cluster = Hex2Dec(attribute_ext_3.substr(i, fragment_cluster_parse * 2));
                        }
                        for (long long j = fragment_cluster * sectors_per_cluster; cluster_count_parse > 0 && j < (fragment_cluster + cluster_count) * sectors_per_cluster; j++) {
                            ReadSector(_drive, j * bytes_per_sector, tempSector, 512);
                            string tempData(tempSector, tempSector + sizeof tempSector / sizeof tempSector[0]);
                            data += tempData;
                        }
                    }
                    cout << data << endl;
                    cout << "Size: " << total_clusters * sectors_per_cluster * bytes_per_sector << endl;
                }
            }
            temp_offset = Dec2Hex(Hex2Dec(temp_offset) + attribute_size);
        }
    }
    return data;
}

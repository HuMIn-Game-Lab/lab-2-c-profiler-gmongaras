import React, { useState } from 'react';
import BarChart from './components/BarChart';
import TimelineChart from './components/TimelineChart';
import './styles.css';

const App = () => {
    const [data, setData] = useState([]);
    const [filteredData, setFilteredData] = useState([]);
    const [sectionNames, setSectionNames] = useState([]);
    const [selectedSection, setSelectedSection] = useState('');
    const [logTransform, setLogTransform] = useState(false); // State to manage log transform

    const handleFileUpload = (event) => {
        const file = event.target.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = (e) => {
                const jsonData = JSON.parse(e.target.result).profiler;
                setData(jsonData);
                setFilteredData(jsonData);
                const sections = [...new Set(jsonData.map(d => d['Section Name']))];
                setSectionNames(sections);
            };
            reader.readAsText(file);
        }
    };

    const handleSectionFilter = (event) => {
        const section = event.target.value;
        setSelectedSection(section);
        if (section === "") {
            setFilteredData(data);
        } else {
            const filtered = data.filter(d => d['Section Name'] === section);
            setFilteredData(filtered);
        }
    };

    const handleLogTransformToggle = (event) => {
        setLogTransform(event.target.checked); // Toggle log transform on or off
    };

    return (
        <div className="App">
            <h1>Profiler Output Visualization Tool</h1>

            {/* File Upload */}
            <div>
                <input type="file" accept=".json" onChange={handleFileUpload} />
            </div>

            {/* Log Transform Checkbox */}
            <div>
                <label>
                    <input type="checkbox" onChange={handleLogTransformToggle} />
                    Apply Log Transform to Average, Max, Min, and Total Time
                </label>
            </div>

            {/* Filter Section */}
            <div>
                <label htmlFor="filterSection">Filter by Section Name:</label>
                <select id="filterSection" value={selectedSection} onChange={handleSectionFilter}>
                    <option value="">All Sections</option>
                    {sectionNames.map((section, index) => (
                        <option key={index} value={section}>{section}</option>
                    ))}
                </select>
            </div>

            {/* Charts */}
            <div className="chart-container">
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Avg Time" metricLabel="Average Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Max Time" metricLabel="Max Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Min Time" metricLabel="Min Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Total Time" metricLabel="Total Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <TimelineChart data={filteredData} />}
            </div>
        </div>
    );
};

export default App;

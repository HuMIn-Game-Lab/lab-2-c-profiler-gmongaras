import React, { useRef, useEffect } from 'react';
import * as d3 from 'd3';

const TimelineChart = ({ data }) => {
    const svgRef = useRef();

    useEffect(() => {
        const svg = d3.select(svgRef.current);
        svg.selectAll('*').remove();  // Clear old elements to avoid overlap

        const margin = { top: 40, right: 30, bottom: 80, left: 80 }; // Adjusted left margin for Y-axis label
        const width = 500 - margin.left - margin.right;
        const height = 400 - margin.top - margin.bottom;

        const timelines = data.map(d => ({
            name: d['Section Name'],  // Using Section Name instead of Function Name
            timeline: d['Timeline'].map((time, index) => ({
                call: index + 1,
                time: +time
            }))
        }));

        const x = d3.scaleLinear()
            .domain([1, d3.max(timelines, d => d3.max(d.timeline, t => t.call)) || 2]) // Ensure domain covers at least two points
            .range([0, width]);

        const y = d3.scaleLinear()
            .domain([0, d3.max(timelines, d => d3.max(d.timeline, t => t.time)) || 1]) // Ensure domain handles single values
            .range([height, 0]);

        const color = d3.scaleOrdinal(d3.schemeCategory10)
            .domain(timelines.map(d => d.name));

        const chart = svg.append("g")
            .attr("transform", `translate(${margin.left},${margin.top})`);

        // Add X axis
        chart.append("g")
            .attr("transform", `translate(0,${height})`)
            .call(d3.axisBottom(x));

        // Add Y axis
        chart.append("g")
            .call(d3.axisLeft(y));

        // Add X axis label
        svg.append("text")
            .attr("text-anchor", "middle")
            .attr("x", width / 2 + margin.left)
            .attr("y", height + margin.top + 50)
            .text("Number of Executions")
            .attr("fill", "#f5f5f5");

        // Add Y axis label with padding to avoid overlap
        svg.append("text")
            .attr("text-anchor", "middle")
            .attr("x", -height / 2)
            .attr("y", margin.left / 3)  // Increased padding
            .attr("transform", "rotate(-90)")
            .text("Total Time")
            .attr("fill", "#f5f5f5");

        // Create line generator
        const line = d3.line()
            .x(d => x(d.call))
            .y(d => y(d.time));

        // Conditionally draw either a line or a point
        timelines.forEach((timelineData) => {
            if (timelineData.timeline.length > 1) {
                // Draw a line if there are multiple points
                chart.append("path")
                    .datum(timelineData.timeline)
                    .attr("class", "line")
                    .attr("fill", "none")
                    .attr("stroke", color(timelineData.name))
                    .attr("stroke-width", 2)
                    .attr("d", line);
            } else {
                // Draw a single point if there's only one data point
                chart.append("circle")
                    .attr("cx", x(timelineData.timeline[0].call))
                    .attr("cy", y(timelineData.timeline[0].time))
                    .attr("r", 4)
                    .attr("fill", color(timelineData.name));
            }
        });

        // Add legend
        const legend = chart.selectAll(".legend")
            .data(timelines)
            .join("g")
            .attr("class", "legend")
            .attr("transform", (d, i) => `translate(10,${i * 20})`);

        legend.append("rect")
            .attr("x", width - 18)
            .attr("width", 18)
            .attr("height", 18)
            .style("fill", d => color(d.name));

        legend.append("text")
            .attr("x", width - 24)
            .attr("y", 9)
            .attr("dy", ".35em")
            .style("text-anchor", "end")
            .attr("fill", "#f5f5f5")
            .text(d => d.name);

    }, [data]);

    return (
        <svg ref={svgRef} width="500" height="500"></svg>
    );
};

export default TimelineChart;

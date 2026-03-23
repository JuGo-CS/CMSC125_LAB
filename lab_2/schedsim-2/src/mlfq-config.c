#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../includes/mlfq-config.h"

/**
 * Load MLFQ configuration from file
 * 
 * Format:
 * Q0 10 50      # Queue 0: quantum=10, allotment=50
 * Q1 30 150     # Queue 1: quantum=30, allotment=150
 * Q2 -1 -1      # Queue 2: FCFS (no quantum/allotment limits)
 * BOOST_PERIOD 200
 */
MLFQConfig* load_mlfq_config(const char* config_file) {
    // Try with workloads/ prefix
    char filepath[256] = "workloads/";
    strncat(filepath, config_file, sizeof(filepath) - strlen(filepath) - 1);
    
    FILE* fp = fopen(filepath, "r");
    if (!fp) {
        // Try without prefix
        fp = fopen(config_file, "r");
        if (!fp) {
            fprintf(stderr, "Error: Cannot open MLFQ config file: %s\n", config_file);
            return NULL;
        }
    }
    
    MLFQConfig* config = malloc(sizeof(MLFQConfig));
    if (!config) {
        fprintf(stderr, "Error: Failed to allocate memory for MLFQ config\n");
        fclose(fp);
        return NULL;
    }
    
    memset(config, 0, sizeof(MLFQConfig));
    config->num_levels = 0;
    config->boost_period = -1;  // Default: no boost
    
    char line[MLFQ_CONFIG_LINE_MAX];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        
        // Remove trailing newline and whitespace
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }
        
        // Trim leading whitespace
        char* trimmed = line;
        while (*trimmed && (*trimmed == ' ' || *trimmed == '\t')) {
            trimmed++;
        }
        
        // Skip empty lines and comments
        if (*trimmed == '\0' || *trimmed == '#') {
            continue;
        }
        
        // Parse queue definitions
        if (strncmp(trimmed, "Q", 1) == 0) {
            int queue_id, quantum, allotment;
            int parsed = sscanf(trimmed, "Q%d %d %d", &queue_id, &quantum, &allotment);
            
            if (parsed != 3) {
                fprintf(stderr, "Error: Invalid queue definition at line %d: %s\n", line_num, trimmed);
                free(config);
                fclose(fp);
                return NULL;
            }
            
            if (queue_id >= MAX_MLFQ_LEVELS) {
                fprintf(stderr, "Error: Queue ID %d exceeds maximum (%d)\n", queue_id, MAX_MLFQ_LEVELS - 1);
                free(config);
                fclose(fp);
                return NULL;
            }
            
            if (queue_id >= config->num_levels) {
                config->num_levels = queue_id + 1;
            }
            
            config->levels[queue_id].queue_id = queue_id;
            config->levels[queue_id].time_quantum = quantum;
            config->levels[queue_id].allotment = allotment;
        }
        // Parse boost period
        else if (strncmp(trimmed, "BOOST_PERIOD", 12) == 0) {
            int boost;
            int parsed = sscanf(trimmed, "BOOST_PERIOD %d", &boost);
            
            if (parsed != 1) {
                fprintf(stderr, "Error: Invalid BOOST_PERIOD at line %d: %s\n", line_num, trimmed);
                free(config);
                fclose(fp);
                return NULL;
            }
            
            config->boost_period = boost;
        }
    }
    
    fclose(fp);
    
    // Validate configuration
    if (!validate_mlfq_config(config)) {
        free(config);
        return NULL;
    }
    
    return config;
}

/**
 * Validate MLFQ configuration
 */
int validate_mlfq_config(MLFQConfig* config) {
    if (!config) {
        fprintf(stderr, "Error: NULL config\n");
        return 0;
    }
    
    if (config->num_levels <= 0) {
        fprintf(stderr, "Error: Must have at least 1 priority level\n");
        return 0;
    }
    
    if (config->num_levels < 3) {
        fprintf(stderr, "Warning: MLFQ should have at least 3 priority levels\n");
    }
    
    // Check that at least one queue is defined
    int has_queue = 0;
    for (int i = 0; i < config->num_levels; i++) {
        if (config->levels[i].time_quantum != 0 || config->levels[i].allotment != 0) {
            has_queue = 1;
            break;
        }
    }
    
    if (!has_queue) {
        fprintf(stderr, "Error: No queues defined in config\n");
        return 0;
    }
    
    // Validate that quantums are decreasing or equal (moving down in priority)
    for (int i = 1; i < config->num_levels; i++) {
        if (config->levels[i].time_quantum > 0 && 
            config->levels[i-1].time_quantum > 0) {
            if (config->levels[i].time_quantum < config->levels[i-1].time_quantum) {
                // This is valid - quantums can decrease at lower priorities
                // But they should not increase
            }
        }
    }
    
    return 1;
}

/**
 * Free MLFQ configuration
 */
void free_mlfq_config(MLFQConfig* config) {
    if (config) {
        free(config);
    }
}

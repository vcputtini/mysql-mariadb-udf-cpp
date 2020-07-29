-- =========================================================================
--
-- The library must be copied to the MySQL/MariaDB plugin directory.
-- Check your Linux distribution for the correct path.
-- To confirm the correct path use: show variables like '%plugin%';
--
-- Syntax:
-- CREATE [OR REPLACE] [AGGREGATE] FUNCTION [IF NOT EXISTS] function_name
--    RETURNS {STRING|INTEGER|REAL|DECIMAL}
--    SONAME shared_library_name
-- =========================================================================

-- USE default;

-- ****************************************************************************
-- libutilities

CREATE OR REPLACE AGGREGATE FUNCTION count_if RETURNS INTEGER SONAME 'libutilities.so';
CREATE OR REPLACE AGGREGATE FUNCTION sum_if RETURNS REAL SONAME 'libutilities.so';
CREATE OR REPLACE AGGREGATE FUNCTION avg_if RETURNS REAL SONAME 'libutilities.so';

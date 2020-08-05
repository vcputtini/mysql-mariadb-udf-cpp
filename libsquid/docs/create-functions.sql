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

CREATE OR REPLACE FUNCTION log_squid RETURNS STRING SONAME 'liblogsquid.so';

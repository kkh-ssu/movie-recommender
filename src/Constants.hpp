#pragma once
 
// ── 평점 범위 ─────────────────────────────────────────────────────────────────
constexpr double MIN_SCORE = 0.0;
constexpr double MAX_SCORE = 5.0;
 
// ── 출력 포맷 ─────────────────────────────────────────────────────────────────
constexpr int RATING_PRECISION = 1;   // setprecision(1)
 
// ── 추천 알고리즘 ─────────────────────────────────────────────────────────────
constexpr int    MIN_RATING_THRESHOLD            = 3;    // 협업 필터링 적용 최소 평점 수
constexpr int    DEFAULT_AVERAGE_RECOMMEND_COUNT = 3;   // 평점 부족 시 평균 기반 추천 수
constexpr int    DEFAULT_TOP_N                   = 5;    // 기본 추천 결과 수
constexpr int    MAX_PICKS_PER_USER              = 2;    // 유사 유저 1명당 최대 추천 수
constexpr double SIMILARITY_WEIGHT               = 10.0; // 공통 영화 1편당 유사도 가중치
 
// ── 장르 TOP 추천 ─────────────────────────────────────────────────────────────
constexpr int TOP_GENRE_COUNT = 3;   // 장르별 TOP N 출력 수

// ── 영화 연도 범위 ────────────────────────────────────────────────────────────
constexpr int MIN_RELEASE_YEAR = 1895; // 뤼미에르 형제 최초 상영 연도
constexpr int MAX_RELEASE_YEAR = 2026; // 현재 연도

// ── 페이지네이션 ──────────────────────────────────────────────────────────────
constexpr int PAGE_SIZE = 10;          // 한 페이지에 출력할 항목 수
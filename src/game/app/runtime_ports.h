#ifndef PROJECTSKYLAKE_RUNTIME_PORTS_H
#define PROJECTSKYLAKE_RUNTIME_PORTS_H

class IClock;
class IContentSource;
class RandomStreamFactory;
class ISaveCodec;
class ISaveStore;
class MigrationRegistry;
class SaveMapper;

struct RuntimePorts {
    IContentSource&            m_content;
    ISaveStore&                m_saves;
    const ISaveCodec&          m_codec;
    const MigrationRegistry&   m_migrations;
    const SaveMapper&          m_mapper;
    const RandomStreamFactory& m_rng;
    const IClock&              m_clock;
};

#endif //PROJECTSKYLAKE_RUNTIME_PORTS_H
